TEMPLATE = app
TARGET   = Prog
CONFIG  *=  warn_on debug arv
CONFIG *= qglviewer glut #apig

ARV = camera tracker
#APIG = 
#HEADERS = sources/*.h
SOURCES = sources/*.cpp
OBJECTS_DIR = .obj

