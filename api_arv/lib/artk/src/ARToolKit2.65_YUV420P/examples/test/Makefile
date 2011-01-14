INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

LDFLAG=-L/usr/X11R6/lib -L$(LIB_DIR)
LIBS= -lARgsub -lARvideo -lAR -lglut -lGLU -lGL -lXi -lXmu -lX11 -lm
CFLAG= -O -I/usr/X11R6/include -I$(INC_DIR)


OBJS = 
HEADDERS = 

all: graphicsTest

graphicsTest: graphicsTest.o $(OBJS)
	cc -o graphicsTest graphicsTest.o $(OBJS) $(LDFLAG) $(LIBS)

graphicsTest.o: graphicsTest.c $(HEADDERS)
	cc -c $(CFLAG) graphicsTest.c

clean:
	rm -f *.o
	rm -f graphicsTest

allclean:
	rm -f *.o
	rm -f graphicsTest
	rm -f Makefile
