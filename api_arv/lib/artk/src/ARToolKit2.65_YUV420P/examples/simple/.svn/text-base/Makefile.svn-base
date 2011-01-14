INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

LDFLAG=-L/usr/X11R6/lib -L$(LIB_DIR)
LIBS= -lARgsub -lARvideo -lAR -lglut -lGLU -lGL -lXi -lXmu -lX11 -lm
CFLAG= -O -I/usr/X11R6/include -I$(INC_DIR)

OBJS =
HEADDERS =

all: $(BIN_DIR)/simpleTest

$(BIN_DIR)/simpleTest: simpleTest.o $(OBJS)
	cc -o $(BIN_DIR)/simpleTest simpleTest.o $(OBJS) $(LDFLAG) $(LIBS)

simpleTest.o: simpleTest.c $(HEADDERS)
	cc -c $(CFLAG) simpleTest.c

clean:
	rm -f *.o
	rm -f $(BIN_DIR)/simpleTest

allclean:
	rm -f *.o
	rm -f $(BIN_DIR)/simpleTest
	rm -f Makefile
