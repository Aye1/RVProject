if BUILD_JS

lib_LIBRARIES = libplibjs.a

include_HEADERS = js.h

libplibjs_a_SOURCES = js.cxx jsLinux.cxx jsLinuxOld.cxx jsMacOS.cxx \
                             jsMacOSX.cxx jsWindows.cxx jsBSD.cxx   \
                             jsNone.cxx

INCLUDES = -I$(top_srcdir)/src/util

endif

EXTRA_DIST = js.dsp


