# Generated automatically from Makefile.in by configure.
INSTALL=/usr/bin/install -c
LN_S=ln -s
DESTDIR=/usr/local/
DESTINC=$(DESTDIR)include
DESTLIB=$(DESTDIR)lib
MAKE=gmake
MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=7
VERSION=$(MAJOR_VERSION).$(MINOR_VERSION).$(MICRO_VERSION)
SO_EXT=so
DO_MINOR_LIBS=yes
INSTALL_DEPS=all

# remind me to look up how to else in a makefile
ifeq ($(DO_MINOR_LIBS),yes)
INSTALL_DEPS+=install_minor_libs
endif

ifeq ($(DO_MINOR_LIBS),no)
INSTALL_DEPS+=install_major_lib_only
endif

all: configure
	cd jlib && $(MAKE) all
	cd src  && $(MAKE) all
	cd doc  && $(MAKE) all

tags:
	ctags `find . -name "*.[ch]" -print`

etags:
	etags `find . -name "*.[ch]" -print`

clean:
	cd jlib && $(MAKE) clean
	cd doc && $(MAKE) clean
	cd src && $(MAKE) clean
	cd audioconvert && $(MAKE) clean

distclean:
	cd jlib && $(MAKE) distclean
	cd doc && $(MAKE) distclean
	cd src && $(MAKE) distclean
	cd audioconvert && $(MAKE) distclean
	cd test && $(MAKE) distclean
	-rm -Rf Makefile config.log config.cache config.status tags gmon.out

test:	all
	cd test && $(MAKE)

install_minor_libs:
	$(INSTALL) -d -m 755 $(DESTLIB)
	$(INSTALL) -m 755 src/libopenal.$(SO_EXT).$(VERSION) $(DESTLIB)
	$(LN_S) -f $(DESTLIB)/libopenal.$(SO_EXT).$(VERSION) $(DESTLIB)/libopenal.$(SO_EXT).$(MAJOR_VERSION)
	$(LN_S) -f $(DESTLIB)/libopenal.$(SO_EXT).$(VERSION) $(DESTLIB)/libopenal.so

install_major_lib_only:
	$(INSTALL) -d -m 755 $(DESTLIB)
	$(INSTALL) -m 755 src/libopenal.$(SO_EXT) $(DESTLIB)

install: $(INSTALL_DEPS)
	$(INSTALL) -d -m 755 $(DESTINC)
	$(INSTALL) -d -m 755 $(DESTINC)/AL
	$(INSTALL) -d -m 755 $(DESTLIB)
	$(INSTALL) -m 755 src/libopenal.a $(DESTLIB)
	$(INSTALL) -m 444 ../include/AL/al.h $(DESTINC)/AL
	$(INSTALL) -m 444 ../include/AL/alc.h $(DESTINC)/AL
	$(INSTALL) -m 444 ../include/AL/alu.h $(DESTINC)/AL
	$(INSTALL) -m 444 ../include/AL/alut.h $(DESTINC)/AL
	$(INSTALL) -m 444 ../include/AL/altypes.h $(DESTINC)/AL
	$(INSTALL) -m 444 ../include/AL/alctypes.h $(DESTINC)/AL
	$(INSTALL) -m 444 ../include/AL/alutypes.h $(DESTINC)/AL
	$(INSTALL) -m 444 ../include/AL/aluttypes.h $(DESTINC)/AL
	$(INSTALL) -m 444 include/AL/alext.h $(DESTINC)/AL
	$(INSTALL) -m 444 include/AL/alexttypes.h $(DESTINC)/AL

configure: configure.in
	@echo "configure.in newer than configure...Please re-./configure"
	autoconf
	@exit 1

