#############################################################################
# Makefile for building: Aye1.app/Contents/MacOS/Aye1
# Generated by qmake (2.01a) (Qt 4.6.2) on: jeu. d�c. 16 22:06:57 2010
# Project:  Aye1.pro
# Template: app
# Command: /usr/bin/qmake -macx -o Aye1.xcodeproj/project.pbxproj Aye1.pro
#############################################################################

MOC       = /Developer/Tools/Qt/moc
UIC       = /Developer/Tools/Qt/uic
LEX       = flex
LEXFLAGS  = 
YACC      = yacc
YACCFLAGS = -d
DEFINES       = -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
INCPATH       = -I/usr/local/Qt4.6/mkspecs/macx-xcode -I. -I/Library/Frameworks/QtCore.framework/Versions/4/Headers -I/usr/include/QtCore -I/Library/Frameworks/QtGui.framework/Versions/4/Headers -I/usr/include/QtGui -I/usr/include -I. -Idependencies -I. -I. -I/usr/local/include -I/System/Library/Frameworks/CarbonCore.framework/Headers -F/Library/Frameworks
DEL_FILE  = rm -f
MOVE      = mv -f

IMAGES = 
PARSERS =
preprocess: $(PARSERS) compilers
clean preprocess_clean: parser_clean compiler_clean

parser_clean:
mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compilers: ./moc_AGHMainWindow.cpp ./ui_AGHMainWindow.h
compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_moc_header_make_all: moc_AGHMainWindow.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_AGHMainWindow.cpp
moc_AGHMainWindow.cpp: AGHFile.h \
		AGHMainWindow.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ AGHMainWindow.h -o moc_AGHMainWindow.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_uic_make_all: ui_AGHMainWindow.h
compiler_uic_clean:
	-$(DEL_FILE) ui_AGHMainWindow.h
ui_AGHMainWindow.h: AGHMainWindow.ui
	/Developer/Tools/Qt/uic AGHMainWindow.ui -o ui_AGHMainWindow.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

