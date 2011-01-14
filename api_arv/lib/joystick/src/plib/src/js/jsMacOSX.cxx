#include "js.h"

#ifdef UL_MAC_OSX

#include <mach/mach_error.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <IOKit/IOCFPlugIn.h>
#include <CoreFoundation/CoreFoundation.h>

#ifdef MACOS_10_0_4
#	include <IOKit/hidsystem/IOHIDUsageTables.h>
#else
/* The header was moved here in MacOS X 10.1 */
#	include <Kernel/IOKit/hidsystem/IOHIDUsageTables.h>
#endif

int jsJoystick::kNumDevices = 32 ;
int jsJoystick::numDevices = -1;
io_object_t jsJoystick::ioDevices[kNumDevices];

jsJoystick::jsJoystick(int ident) :
	id(ident), 
	error(JS_FALSE),
	num_axes(0),
	num_buttons(0)
{
	if (numDevices < 0) {
		// do first-time init (since we can't over-ride jsInit, hmm
		numDevices = 0;
		
		mach_port_t masterPort;
		IOReturn rv = IOMasterPort(bootstrap_port, &masterPort);
		if (rv != kIOReturnSuccess) {
			ulSetError(UL_WARNING, "error getting master Mach port");
			return;
		}
		findDevices(masterPort);
	}
	
	if (ident >= numDevices) {
		setError();
		return;
	}
	
	// get the name now too
	CFDictionaryRef properties = getCFProperties(ioDevices[id]);
	CFTypeRef ref = CFDictionaryGetValue (properties, CFSTR(kIOHIDProductKey));
	if (!ref)
		ref = CFDictionaryGetValue (properties, CFSTR("USB Product Name"));
			
	if (!ref || !CFStringGetCString ((CFStringRef) ref, name, 128, CFStringGetSystemEncoding ())) {
		ulSetError(UL_WARNING, "error getting device name");
		name[0] = '\0';
	}
		
	open();
}

/** open the IOKit connection, enumerate all the HID devices, add their
interface references to the static array. We then use the array index
as the device number when we come to open() the joystick. */

void jsJoystick::findDevices(mach_port_t masterPort)
{
	CFMutableDictionaryRef hidMatch = NULL;
	IOReturn rv = kIOReturnSuccess;
	io_iterator_t hidIterator;
	
	// build a dictionary matching HID devices
	hidMatch = IOServiceMatching(kIOHIDDeviceKey);
	
	rv = IOServiceGetMatchingServices(masterPort, hidMatch, &hidIterator);
	if (rv != kIOReturnSuccess || !hidIterator) {
		ulSetError(UL_WARNING, "no joystick (HID) devices found");
		return;
	}
	
	// iterate
	io_object_t ioDev;
	
	while ((ioDev = IOIteratorNext(hidIterator))) {
	// filter out keyboard and mouse devices
		CFDictionaryRef properties = getCFProperties(ioDev);
		long usage, page;
		
		CFTypeRef refPage = CFDictionaryGetValue (properties, CFSTR(kIOHIDPrimaryUsagePageKey));
		CFTypeRef refUsage = CFDictionaryGetValue (properties, CFSTR(kIOHIDPrimaryUsageKey));
		CFNumberGetValue((CFNumberRef) refUsage, kCFNumberLongType, &usage);
		CFNumberGetValue((CFNumberRef) refPage, kCFNumberLongType, &page);
		
		// exclude keyboard / mouse devices
		if ((page == kHIDPage_GenericDesktop) && 
			((usage == kHIDUsage_GD_Keyboard) || (usage == kHIDUsage_GD_Mouse))
			)
			continue;
	
		// add it to the array
		ioDevices[numDevices++] = ioDev;
	}
	
	IOObjectRelease(hidIterator);
}

void jsJoystick::open()
{
	if (id >= numDevices) {
		ulSetError(UL_WARNING, "device index out of range in jsJoystick::open");
		return;
	}
	
	// create device interface
	IOReturn rv;
	SInt32 score;
	IOCFPlugInInterface **plugin;

	rv = IOCreatePlugInInterfaceForService(ioDevices[id], 
		kIOHIDDeviceUserClientTypeID,
		kIOCFPlugInInterfaceID,
		&plugin, &score);
		
	if (rv != kIOReturnSuccess) {
		ulSetError(UL_WARNING, "error creting plugin for io device");
		return;
	}
	
	HRESULT pluginResult = (*plugin)->QueryInterface(plugin, 
		CFUUIDGetUUIDBytes(kIOHIDDeviceInterfaceID), &(LPVOID) hidDev);
		
	if (pluginResult != S_OK)
		ulSetError(UL_WARNING, "QI-ing IO plugin to HID Device interface failed");

	(*plugin)->Release(plugin); // don't leak a ref
	if (hidDev == NULL) return;
		
	// store the interface in this instance
	rv = (*hidDev)->open(hidDev, 0);
	if (rv != kIOReturnSuccess) {
		ulSetError(UL_WARNING, "error opening device interface");
		return;
	}
	
	CFDictionaryRef props = getCFProperties(ioDevices[id]);
			
	// recursively enumerate all the bits
	CFTypeRef topLevelElement = 
		CFDictionaryGetValue (props, CFSTR(kIOHIDElementKey));
	enumerateElements(topLevelElement);
	
	CFRelease(props);
}

CFDictionaryRef jsJoystick::getCFProperties(io_object_t ioDev)
{
	IOReturn rv;
	CFMutableDictionaryRef cfProperties;

#if 0	
	// comment copied from darwin/SDL_sysjoystick.c
	/* Mac OS X currently is not mirroring all USB properties to HID page so need to look at USB device page also
	 * get dictionary for usb properties: step up two levels and get CF dictionary for USB properties
	 */
	 
	io_registry_entry_t parent1, parent2;
	
	rv = IORegistryEntryGetParentEntry (ioDev, kIOServicePlane, &parent1);
	if (rv != kIOReturnSuccess) {
		ulSetError(UL_WARNING, "error getting device entry parent");
		return NULL;
	}
	
	rv = IORegistryEntryGetParentEntry (parent1, kIOServicePlane, &parent2);
	if (rv != kIOReturnSuccess) {
		ulSetError(UL_WARNING, "error getting device entry parent 2");
		return NULL;
	}
	
#endif
	rv = IORegistryEntryCreateCFProperties( ioDev /*parent2*/, 
		&cfProperties, kCFAllocatorDefault, kNilOptions);
	if (rv != kIOReturnSuccess || !cfProperties) {
		ulSetError(UL_WARNING, "error getting device properties");
		return NULL;
	}
	
	return cfProperties;
}

void jsJoystick::close()
{
	(*hidDev)->close(hidDev);
}

void jsJoystick::elementEnumerator( const void *element, void* vjs)
{
	if (CFGetTypeID((CFTypeRef) element) != CFDictionaryGetTypeID()) {
		ulSetError(UL_FATAL, "element enumerator passed non-dictionary value");
		return;
	}
	
	static_cast<jsJoystick*>(vjs)->parseElement((CFDictionaryRef) element);		
}

/** element enumerator function : pass NULL for top-level*/
void jsJoystick::enumerateElements(CFTypeRef element)
{
	assert(CFGetTypeID(element) == CFArrayGetTypeID());
	
	CFRange range = {0, CFArrayGetCount ((CFArrayRef)element)};
	CFArrayApplyFunction((CFArrayRef) element, range, 
		&jsJoystick::elementEnumerator, this);
}

void jsJoystick::parseElement(CFDictionaryRef element)
{
	CFTypeRef refPage = CFDictionaryGetValue ((CFDictionaryRef) element, CFSTR(kIOHIDElementUsagePageKey));
	CFTypeRef refUsage = CFDictionaryGetValue ((CFDictionaryRef) element, CFSTR(kIOHIDElementUsageKey));

	long type, page, usage;
	
	CFNumberGetValue((CFNumberRef)
		CFDictionaryGetValue ((CFDictionaryRef) element, CFSTR(kIOHIDElementTypeKey)), 
		kCFNumberLongType, &type);
		
	switch (type) {
	case kIOHIDElementTypeInput_Misc:
	case kIOHIDElementTypeInput_Axis:
	case kIOHIDElementTypeInput_Button:
		printf("got input element...");
		CFNumberGetValue((CFNumberRef) refUsage, kCFNumberLongType, &usage);
		CFNumberGetValue((CFNumberRef) refPage, kCFNumberLongType, &page);
		
		if (page == kHIDPage_GenericDesktop) {		
			switch (usage) /* look at usage to determine function */
			{
				case kHIDUsage_GD_X:
				case kHIDUsage_GD_Y:
				case kHIDUsage_GD_Z:
				case kHIDUsage_GD_Rx:
				case kHIDUsage_GD_Ry:
				case kHIDUsage_GD_Rz:
				case kHIDUsage_GD_Slider: // for throttle / trim controls
					printf(" axis\n");
					addAxisElement((CFDictionaryRef) element);
					break;
					
				case kHIDUsage_GD_Hatswitch:
					printf(" hat\n");
					addHatElement((CFDictionaryRef) element);
					break;
					
				default:
					printf("input type element has weird usage (%x)\n", usage);
		break;
			}					
		} else if (page == kHIDPage_Button) {
			printf(" button\n");
			addButtonElement((CFDictionaryRef) element);
		} else
			printf("input type element has weird page (%x)\n", page);
		break;

	case kIOHIDElementTypeCollection:
		enumerateElements(
			CFDictionaryGetValue(element, CFSTR(kIOHIDElementKey))
		);
		break;
		
	default:
		break;
	}	
}

void jsJoystick::addAxisElement(CFDictionaryRef axis)
{
	long cookie, lmin, lmax;
	CFNumberGetValue ((CFNumberRef)
		CFDictionaryGetValue (axis, CFSTR(kIOHIDElementCookieKey)), 
		kCFNumberLongType, &cookie);
	
	int index = num_axes++;
	
	axisCookies[index] = (IOHIDElementCookie) cookie;
	
	CFNumberGetValue ((CFNumberRef)
		CFDictionaryGetValue (axis, CFSTR(kIOHIDElementMinKey)), 
		kCFNumberLongType, &lmin);
		
	CFNumberGetValue ((CFNumberRef)
		CFDictionaryGetValue (axis, CFSTR(kIOHIDElementMaxKey)), 
		kCFNumberLongType, &lmax);
		
	min[index] = lmin;
	max[index] = lmax;
	dead_band[index] = 0.0;
	saturate[index] = 1.0;
	center[index] = (lmax - lmin) * 0.5 + lmin;
}

void jsJoystick::addButtonElement(CFDictionaryRef button)
{
	long cookie;
	CFNumberGetValue ((CFNumberRef)
		CFDictionaryGetValue (button, CFSTR(kIOHIDElementCookieKey)), 
		kCFNumberLongType, &cookie);
		
	buttonCookies[num_buttons++] = (IOHIDElementCookie) cookie;
	// anything else for buttons?
}

void jsJoystick::addHatElement(CFDictionaryRef button)
{
	//hatCookies[num_hats++] = (IOHIDElementCookie) cookie;
	// do we map hats to axes or buttons?
}

void jsJoystick::rawRead(int *buttons, float *axes)
{
	*buttons = 0;
	 IOHIDEventStruct hidEvent;
	
	for (int b=0; b<num_buttons; ++b) {
		(*hidDev)->getElementValue(hidDev, buttonCookies[b], &hidEvent);
		if (hidEvent.value)
			*buttons |= 1 << b; 
	}
	
	for (int a=0; a<num_axes; ++a) {
		(*hidDev)->getElementValue(hidDev, axisCookies[a], &hidEvent);
		axes[a] = hidEvent.value;
	}
}

#endif
