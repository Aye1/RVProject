######################################################################
# Automatically generated by qmake (2.01a) ven. janv. 14 15:04:36 2011
######################################################################

TEMPLATE = app
TARGET   = rt
CONFIG  += qt opengl warn_on release thread arv
DEPENDPATH += .
INCLUDEPATH += .
LIBS += -L/usr/local/lib/ -lSDL -lSDL_image -g
ARV= wii fob

# Input
HEADERS += AGHFile.h \
           AGHMainWindow.h \
           baguette.h \
           color.h \
           elementBat.h \
           hit.h \
           light.h \
           material.h \
           scene.h \
           viewerD.h \
		   viewerG.h \
		   widgetScene.h \
		   touche.h \
		   Skybox.h \
		   sdlglutils.h \
		   myfob.h \
		   wiim.h \
	   AGHListeTouches.h
FORMS += AGHMainWindow.ui
SOURCES += AGHFile.cpp \
           AGHMainWindow.cpp \
           baguette.cpp \
           elementBat.cpp \
           light.cpp \
           main.cpp \
           material.cpp \
           scene.cpp \
           viewerD.cpp \
		   viewerG.cpp \
		   widgetScene.cpp \
		   touche.cpp \
		   Skybox.cpp \
		   sdlglutils.cpp \
		   myfob.cpp \
		   wiim.cpp \
	   AGHListeTouches.cpp

include( config.pri )
