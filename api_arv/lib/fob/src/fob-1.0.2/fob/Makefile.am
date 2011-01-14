AUTOMAKE_OPTIONS = 1.6

FOB_CPP = fob.cpp
MATH_CPP = quaternion.cpp vector.cpp matrix.cpp

#AM_CXXFLAGS = @CXXFLAGS@ @WERROR@ 
#AM_LDFLAGS = @LDFLAGS@

INCLUDES = -I$(top_srcdir) -I$(top_builddir)

lib_LTLIBRARIES = libfob-1.0.la
libfob_1_0_la_SOURCES = $(FOB_CPP) $(MATH_CPP)
libfob_1_0_la_LDFLAGS = -version-info $(LIBFOB_SO_VERSION) $(PTHREAD_LIBS)

libfob_includedir	= $(includedir)/fob
libfob_include_HEADERS = \
	fob.h \
	quaternion.h \
	matrix.h \
	mathematics.h \
	vector.h
