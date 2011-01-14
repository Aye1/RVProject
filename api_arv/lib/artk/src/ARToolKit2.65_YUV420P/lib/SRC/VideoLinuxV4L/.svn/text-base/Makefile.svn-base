#
# For instalation. Change this to your settings.
#
INC_DIR = ../../../include
LIB_DIR = ../..
#
#  compiler
#
CC= cc
CFLAG= -O -I/usr/X11R6/include -I$(INC_DIR)
#
# For making the library
#
AR= ar
ARFLAGS= rs
#
#   products
#
LIB= ${LIB_DIR}/libARvideo.a
INCLUDE= ${INC_DIR}/AR/video.h
#
#   compilation control
#
LIBOBJS= ${LIB}(video.o)

all:		${LIBOBJS}

${LIBOBJS}:	${INCLUDE}

.c.a:
	${CC} -c ${CFLAG} $<
	${AR} ${ARFLAGS} $@ $*.o
	rm -f $*.o

clean:
	rm -f *.o
	rm -f ${LIB}

allclean:
	rm -f *.o
	rm -f ${LIB}
	rm -f Makefile
