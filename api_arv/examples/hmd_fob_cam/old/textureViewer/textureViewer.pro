# Loads an image file and texture maps a polygon with it.

# Pedagogical example that shows how to apply a texture on a 3D object.
# Inpired from the Qt texture example.

# The Qt <code>QImage</code> class and its <code>convertToGLFormat()</code> function are used to
# load any image format. The image is resized so that its dimensions are powers of two if needed.
# Feel free to cut and paste.

TEMPLATE = app
TARGET   = textureViewer
CONFIG  += general qt opengl release thread

HEADERS  = textureViewer.h
SOURCES  = textureViewer.cpp main.cpp

DISTFILES += *.png

#Windows Qt 2.3 users should uncomment the next 2 lines and remove all the remaining lines.
#DEFINES *= QT_DLL QT_THREAD_SUPPORT
#LIBS *= QGLViewer.lib

# The rest of this configuration file is pretty complex since it tries to automatically
# detect system paths and configuration. In your applications, you can probably simply use :
# LIBS *= -lQGLViewer (unix)
# LIBS *= QGLViewer.lib (Visual 6) or QGLViewer139.lib (.NET) (win32)


#                       Unix configuration
# See doc/installUnix.html and doc/examples/index.html for details.
# Same INCLUDE_DIR and LIB_DIR parameters than for the make install.
unix {
  isEmpty( PREFIX ) {
    PREFIX=/usr
  }

  # INCLUDE_DIR
  isEmpty( INCLUDE_DIR ) {
    INCLUDE_DIR = $$PREFIX/include

    !exists( $$INCLUDE_DIR/QGLViewer/qglviewer.h ) {
      exists( ../../../QGLViewer/qglviewer.h ) {
        message( Using ../../.. as INCLUDE_DIR )
        INCLUDE_DIR = ../../..
      }
    }
  }

  !exists( $$INCLUDE_DIR/QGLViewer/qglviewer.h ) {
    message( Unable to find QGLViewer/qglviewer.h in $$INCLUDE_DIR )
    error( Use qmake INCLUDE_DIR=Path/To/QGLViewerHeaderFiles )
  }

  # LIB_NAME
  LIB_NAME = libQGLViewer.so*
  macx|darwin-g++ {
    LIB_NAME = libQGLViewer.*.$$QMAKE_EXTENSION_SHLIB
  }
  hpux {
    LIB_NAME = libQGLViewer.sl*
  }

  # LIB_DIR
  isEmpty( LIB_DIR ) {
    LIB_DIR = $$PREFIX/lib

    !exists( $$LIB_DIR/$$LIB_NAME ) {
      exists( ../../../QGLViewer/$$LIB_NAME ) {
        message( Using ../../../QGLViewer as LIB_DIR )
        macx|darwin-g++ {
          message(  You should add the path to "../../../QGLViewer" to your DYLD_LIBRARY_PATH variable )
        } else {
          message(  You should add the path to "../../../QGLViewer" to your LD_LIBRARY_PATH variable )
        }
        message( See the "Compilation" section in doc/examples/index.html for details )
        LIB_DIR = ../../../QGLViewer
      }
    }
  }

  !exists( $$LIB_DIR/$$LIB_NAME ) {
    message( Unable to find $$LIB_NAME in $$LIB_DIR )
    error( You should run qmake LIB_DIR=Path/To/$$LIB_NAME )
  }

  # Paths were correctly detected
  INCLUDEPATH *= $$INCLUDE_DIR
  DEPENDPATH  *= $$INCLUDE_DIR
  LIBS        *= -L$$LIB_DIR -lQGLViewer

  macx {
    LIBS *= -lobjc
    CONFIG -= thread
  }

  # Remove debugging options
  QMAKE_CFLAGS_RELEASE -= -g
  QMAKE_CXXFLAGS_RELEASE -= -g

  # Hide intermediate files
  MOC_DIR = .moc
  OBJECTS_DIR = .obj

  # Detect Qt version
  !exists( $$(QTDIR)/lib/libqt-mt.* ) {
    exists( $$(QTDIR)/lib/libqt.* ) {
      CONFIG -= thread
      message( Using Qt non threaded version )
    } else {
      message( Unable to find Qt library in $$(QTDIR)/lib. Check your QTDIR )
    }
  }
}


#                    Windows configuration.
# See doc/installWindows.html and doc/examples/index.html for details
win32 {
  # Various compilation flags
  QMAKE_CXXFLAGS = -TP -G6 -W1 -GR -GX -Zi
  # Optimise for speed, and expand any suitable inlines
  QMAKE_CXXFLAGS_RELEASE = -O2 -Ob2

  # Use the Qt DLL version
  DEFINES *= QT_DLL QT_THREAD_SUPPORT

  # Standard zip tree : libQGLViewer is in ../../..
  exists( ../../../QGLViewer/qglviewer.h ) {
    INCLUDEPATH *= ../../..
  }

  exists( ../../../QGLViewer/Debug/QGLViewer*.lib ) {
    LIBPATH = ../../../QGLViewer/Debug
  }

  exists( ../../../QGLViewer/Release/QGLViewer*.lib ) {
    LIBPATH = ../../../QGLViewer/Release
  }

  exists( ../../../QGLViewer/QGLViewer*.lib ) {
    LIBPATH = ../../../QGLViewer
  }

  win32-msvc.net {
    LIBS *= $$LIBPATH/QGLViewer139.lib
  } else {
    LIBS *= $$LIBPATH/QGLViewer.lib
  }
}
