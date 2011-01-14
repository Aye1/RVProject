TEMPLATE = lib
CONFIG *= staticlib
TARGET   = homography
VERSION  = 1.0
CONFIG  *= warn_on release thread

HEADERS  = sources/*.h  sources/NR/*.h
SOURCES  = sources/*.cc sources/NR/*.cc

OBJECTS_DIR = .obj

