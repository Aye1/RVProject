TEMPLATE = app
TARGET   = ../CalibCParam
CONFIG  *=  warn_on release arv

ARV = ext_artk
ARTK = gsub
HEADERS = src/*.h
SOURCES = src/*.c

OBJECTS_DIR = .obj
