INC_DIR= ../../include
LIB_DIR= ../../lib
BIN_DIR= ../../bin

LDFLAG=-L/usr/X11R6/lib -L$(LIB_DIR)
LIBS= -lARgsub -lARvideo -lAR -lglut -lGLU -lGL -lXi -lXmu -lX11 -lm
CFLAG= -O -I/usr/X11R6/include -I$(INC_DIR)


OBJS = object.o draw_object.o
HEADDERS = object.h draw_object.h

all: $(BIN_DIR)/modeTest

$(BIN_DIR)/modeTest: modeTest.o $(OBJS)
	cc -o $(BIN_DIR)/modeTest modeTest.o $(OBJS) $(LDFLAG) $(LIBS)

modeTest.o: modeTest.c $(HEADDERS)
	cc -c $(CFLAG) modeTest.c

object.o: object.c $(HEADDERS)
	cc -c $(CFLAG) object.c

draw_object.o: draw_object.c $(HEADDERS)
	cc -c $(CFLAG) draw_object.c

clean:
	rm -f *.o
	rm -f $(BIN_DIR)/modeTest

allclean:
	rm -f *.o
	rm -f $(BIN_DIR)/modeTest
	rm -f Makefile
