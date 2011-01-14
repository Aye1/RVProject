INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

LDFLAG=-L/usr/X11R6/lib -L$(LIB_DIR)
LIBS= -lARgsub -lARvideo -lAR -lglut -lGLU -lGL -lXi -lXmu -lX11 -lm
CFLAG= -O -I/usr/X11R6/include -I$(INC_DIR)

OBJS =
HEADDERS =

all: $(BIN_DIR)/simpleTest2

$(BIN_DIR)/simpleTest2: simpleTest2.o $(OBJS)
	cc -o $(BIN_DIR)/simpleTest2 simpleTest2.o $(OBJS) $(LDFLAG) $(LIBS)

simpleTest2.o: simpleTest2.c $(HEADDERS)
	cc -c $(CFLAG) simpleTest2.c

clean:
	rm -f *.o
	rm -f $(BIN_DIR)/simpleTest2

allclean:
	rm -f *.o
	rm -f $(BIN_DIR)/simpleTest2
	rm -f Makefile
