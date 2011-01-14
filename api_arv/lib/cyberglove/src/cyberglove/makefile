CC = g++
CC_OPT =	

LIBS =	         -lm 
XLIBS =		-lXm -lXt -lX11

CFLAGS= $(CC_OPT) $(OPT) -DSGI -I./include -o $*.o

OBJS = sample.o

.SUFFIXES : .o .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) $*.cpp


all:	sample

sample: $(OBJS) ./lib/libcyberglove.a
	g++ -L./lib -o sample $(OBJS) -lcyberglove
