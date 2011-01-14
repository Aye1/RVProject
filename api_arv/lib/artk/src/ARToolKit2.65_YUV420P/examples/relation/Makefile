INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

LDFLAG=-L/usr/X11R6/lib -L$(LIB_DIR)
LIBS= -lARgsub -lARvideo -lAR -lglut -lGLU -lGL -lXi -lXmu -lX11 -lm
CFLAG= -O -I/usr/X11R6/include -I$(INC_DIR)

OBJS =
HEADDERS =

all: $(BIN_DIR)/relationTest

$(BIN_DIR)/relationTest: relationTest.o $(OBJS)
	cc -o $(BIN_DIR)/relationTest relationTest.o $(OBJS) $(LDFLAG) $(LIBS)

relationTest.o: relationTest.c $(HEADDERS)
	cc -c $(CFLAG) relationTest.c

clean:
	rm -f *.o
	rm -f $(BIN_DIR)/relationTest

allclean:
	rm -f *.o
	rm -f $(BIN_DIR)/relationTest
	rm -f Makefile
