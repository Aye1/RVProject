TEMPLATE = app
TARGET   = rt
CONFIG  += qt opengl warn_on release thread

HEADERS  = viewer.h   material.h   scene.h   camera.h   light.h   elementBat.h   baguette.h   color.h  hit.h AGHMainWindow.h AGHFile.h
SOURCES  = viewer.cpp material.cpp scene.cpp camera.cpp light.cpp elementBat.cpp baguette.cpp main.cpp AGHMainWindow.h AGHFile.h

include( config.pri )
