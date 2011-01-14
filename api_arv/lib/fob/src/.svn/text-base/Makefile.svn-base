#!smake
#@(#) Makefile -------------- 2000/11/10 Grasset Raphael ----------------------

#description : Generic Makefile for SGI and Linux

#rem: SHELL=/usr/bin/tcsh =>don't use with smake 
#different architecture: hinv -c processor
#list of architectures:
#ip19 ip20 ip21 ip22_4k ip22_5k ip24 ip25 ip26 ip27 ip28 ip30 ip32_5k ip32_10k # faire 3 versions: debug, optimise, release 
# faire generation doc automatique pour release

# -------------------------------- SOURCES ------------------------------------
#executable name
EXECUTABLE=test_sample0

#source to add
SRC=sample0.C 

#obj to add
EXOBJ=

#file to moc
FILE_TO_MOC=

# ------------------------------ ARCHITECTURE ---------------------------------

#OPERATING SYSTEM
OS=-DLinux
#OS = -DIRIX

#ARCHITECTURE
ARCH=-DI386 #LINUX
#ABI N32 pour R4000,R4400,R4600 (-mips3 par defaut)/indy+indigo2+qques 02
#ARCH=-n32 
#ABI N32 pour R5000,R8000,R10000 (par defaut)/O2/piment/ginseng
#ARCH=-n32 -mips4
#ABI N64 pour R10000 R8000 grosse appli/ginseng/piment
#ARCH=-n64 -mips4

#PROGRAM
#compilator to use
CXX=/usr/bin/g++ #LINUX
#CXX=CC
#MOC= /usr/local/bin/moc
CC=gcc #LINUX
#CC=cc
#misc
#AR=ar
RM = rm -f
#AS=as
#LEX=lex
#YACC=yacc

# ------------------------------ VERSION  -------------------------------- 
#DEBUG OPTIONS
WARNING=-Wall -g -Wno-deprecated#LINUX
#all information debug version 
#WARNING=-g -fullwarn -DDEBUG
#optimise debug version (with -0)
#WARNING=-g3 -fullwarn
#optimise version (with -O)
#WARNING=-g0 -w

#OPTIMISATION OPTION
OPTIM=-O2 #LINUX
#debug mode
#OPTIM=-O0
#optimisation 1 mode
#OPTIM=-01
#optimisation 2 mode
#OPTIM=-02
#OPTIM=-0
#optimisation 3 mode (have -LNO)
#OPTIM=-03
#dependency machine optimisation mode
#OPTIM=-Ofast=IP27 #MIPS R10000 (onyx and origin 2000)
#OPTIM=-Ofast=IP32_5k #MIPS R5000 (O2)
#dependency machine optimisation 2 mode (with inter-procedural analyzer)
#OPTIM=-Ofast=IP27 -IPA #MIPS R10000 (onyx and origin 2000)
#OPTIM=-Ofast=IP32_5k -IPA #MIPS R5000 (O2)
#super optimisation mode 
#OPTIM=-O3 -IPA -TARG:platform=ip32_5k -OPT:Olimit=0:roundoff=3:div_split=ON:alias=typed # MIPS R5000 (O2)

#PURIFY OPTIONS
PROF=
#PROF=-pg

# ------------------------------ OPTIONS  ---------------------------------
#COMPILATOR OPTIONS
#misc
{DSOVERSION = sgi4.0 #LINUX
#classical option
#OPTS=
OPTS=-MD #LINUX
#OPTS=-D_SGI_MP_SOURCE -DQT_CLEAN_NAMESPACE
#D_BSD_SOURCE -DSHM -DUSE_X86_ASM -D_REENTRANT -DTHREADS -DPTHREADS 
#preprocessor options
#CPPFLAGS=-LANG:std

#INCLUDE PATH
IPATH=-I/usr/include -I. -I/usr/include  -I/usr/include/g++ -I/usr/X11R6/include 
#IPATH = -I/usr/local/include -I/usr/local/include/QT

#LIB PATH
LDPATH=-L/usr/lib -L/lib -L/usr/X11R6/lib -L.
#LDPATH= -L/usr/local/lib32 -L. -L/usr/lib32/internal 

# ------------------------------ LIBRARY  ---------------------------------

# personnal and classic lib
USER_LIBS = -lfob
#GEN_LIBS = -lm -lfastm -lpthread
GEN_LIBS = -lm -lpthread #LINUX 
#GEN_LIBS = -lm 
#MISC_LIBS=-ldl  -lSM -lICE #LINUX

#media lib
#VL_LIBS  = -lvl -ldmedia -limage
VL_LIBS= -limage

#GUI lib
QT_LIBS  = -lqt

#graphics lib
X_LIBS   = -lXmu -lX11 -lXext 
#X_LIBS   = -lX11 -lXext  -lXmu -lXt -lXm -lXi #LINUX

#3D graphics lib
#MESA_LIBS = -lMesaGLU -lMesaGL 
#OGL_LIBS = -lglut -lGL -lGLU
#OGL_LIBS = -lglut -lGLU -lGL 
OGL_LIBS = -lGLU -lGL 
#IV_LIBS = -lInventor -lInventorXt
PF_LIBS  = -lpfdu -lpfui  -lpfutil -lpf

# user-misc-gui-media-3d-graphic-generic lib
LIBS     =  $(USER_LIBS) $(MISC_LIB) $(OGL_LIBS) $(GEN_LIBS) 
# -------------------------------- RULES  ------------------------------------

LDFLAGS= $(LDPATH) $(LIBS)
DEPENDPATH=$(IFLAGS)
CXXFLAGS=$(OPTIM) $(WARNING) $(OPTS) $(ARCH)  $(OS) $(PROF) $(IPATH)
OBJ=$(SRC:.C=.o) $(EXOBJ) $(FILE_TO_MOC:S/.h/.moc.o/g)

.PATH: 
.SUFFIXES: .C .o .h

.PHONY : clean all #no dependancy necessary for this rules
.DEFAULT : all 	#default commande

#explicite rule	

default: all

all:$(EXECUTABLE)

#Moc QT
$(FILE_TO_MOC:S/.h/.moc.C/g): $(.TARGET:R:R).h
	$(MOC) $> -o $@

#explicite rule	
$(EXECUTABLE):$(OBJ) 
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $(OBJ) $(LDFLAGS)

#implicite rules
.C.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $< 
#	$(C++) $(C++FLAGS) -c $(.IMPSRC)

depend: 
	makedepend $(DEPENDPATH) -Y $(SRC:M*.C)

#clean light:
cleanexe:
	$(RM) $(EXECUTABLE)

cleanobj:
	$(RM)  *.o

#clean medium :   object, exec,
clean: cleanexe cleanobj

#clean hard :  template, object, library, exec
realclean: cleanexe cleanobj
	$(RM) -r ii_files
	$(RM) *.d
	$(RM) *.o
	$(RM)*.a
	$(RM)*.so
	$(RM) *.moc.o
	$(RM) core

clobber:

install:
	echo installation du programme

zip :
	echo archivage des sources 

library :FobAlone.o FobFlock.o FobCommand.o RS232Interface.o
	ar -cr libfob.a FobAlone.o FobFlock.o FobCommand.o RS232Interface.o

library_perf :

samples :

help:
	@echo "make clean 		del  *.o"
	@echo "make all 		build executable"
	@echo "make depend		create dependancy"

# DO NOT DELETE

sampleg2.o: FobAlonePerf.h FobAlone.h FobCommand.h RS232Interface.h
