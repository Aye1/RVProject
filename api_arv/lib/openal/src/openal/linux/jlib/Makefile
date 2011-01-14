# Generated automatically from Makefile.in by configure.
CC        = gcc
CCFLAGS   = -g -O2 -fPIC -Wshadow -Wall -W -Wbad-function-cast -Wcast-qual -Wcast-align -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wimplicit-function-declaration -Waggregate-return -Winline -Wpointer-arith -fno-common -ansi -Wredundant-decls
DEFINES   = 
INC       = -I.
SLIB      = libjlib.a
OBJS      = globals.o  hash.o
MAKE      = gmake
RANLIB    = echo

.SUFFIXES: .c
.c.o:
	$(CC) $(CCFLAGS) $(DEFINES) $(INC) -g -c -o $@ $<

all: $(SLIB)

$(SLIB): $(OBJS)
	ar cru $(SLIB) $(OBJS)
	$(RANLIB) $(SLIB)

install: all

clean:
	-rm -f $(SLIB) $(OBJS) core

distclean: clean
	-rm -f Makefile
