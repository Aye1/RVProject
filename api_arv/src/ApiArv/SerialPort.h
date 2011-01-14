/******************************************
 *  creation :
 *      2008 : Lionel Baboud
 */

#ifndef __ARV_SERIAL_PORT__
#define __ARV_SERIAL_PORT__

namespace arv {

    /** This enumeration if used to reference serial ports of the PC. */
    enum SerialPort {
        PORT0,  /// serial port 0
        PORT1,  /// serial port 1
        PORT2,  /// serial port 2
        PORT3   /// serial port 3
        };
    
    /** This function returns the system name associated with a serial port. */
    char* getSystemName(SerialPort port);
    
    }

#endif

