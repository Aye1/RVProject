CC	= g++

.SUFFIXES : .o .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) $*.cpp

LIBOBJS	= cyberglove.o serial.o

CFLAGS= $(CC_OPT) $(OPT) -DSGI -I../include -o $*.o

all:	libcyberglove.a

cyberglove.o:  cyberglove.cpp ../include/cyberglove.h
serial.o:  serial.cpp ../include/serial.h

libcyberglove.a: $(LIBOBJS) 
	ar r libcyberglove.a $(LIBOBJS)

