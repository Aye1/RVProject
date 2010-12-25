TEMPLATE = app
TARGET   = rt
CONFIG  += qt opengl warn_on release thread

HEADERS  = viewer.h   material.h   scene.h   camera.h   light.h   elementBat.h   color.h  hit.h
SOURCES  = viewer.cpp material.cpp scene.cpp camera.cpp light.cpp elementBat.cpp main.cpp

include( config.pri )
