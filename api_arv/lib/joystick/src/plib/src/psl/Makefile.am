
if BUILD_PSL

lib_LIBRARIES = libplibpsl.a

include_HEADERS = psl.h

libplibpsl_a_SOURCES = psl.cxx pslCodeGen.cxx pslContext.cxx \
                       pslCompiler.cxx pslSymbols.cxx pslToken.cxx \
                       pslExpression.cxx pslProgram.cxx pslDump.cxx \
                       pslError.cxx pslFileIO.cxx pslCompiler.h \
                       pslContext.h pslFileIO.h pslLocal.h \
                       pslOpcodes.h pslSymbol.h

INCLUDES = -I$(top_srcdir)/src/util

endif

EXTRA_DIST = psl.dsp

