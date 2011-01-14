if BUILD_SSGAUX

lib_LIBRARIES = libplibssgaux.a

include_HEADERS = ssgAux.h             \
                  ssgaShapes.h         \
                  ssgaLensFlare.h      \
                  ssgaParticleSystem.h \
                  ssgaWaveSystem.h     \
                  ssgaScreenDump.h     \
                  ssgaSky.h            \
                  ssgaSphere.h         \
                  ssgaFire.h           \
                  ssgaBillboards.h

libplibssgaux_a_SOURCES = ssgAux.cxx               \
                          ssgaShapes.cxx           \
                          ssgaPatch.cxx            \
                          ssgaParticleSystem.cxx   \
                          ssgaWaveSystem.cxx       \
                          ssgaLensFlare.cxx        \
                          ssgaLensFlareTexture.cxx \
                          ssgaFire.cxx             \
                          ssgaScreenDump.cxx       \
                          ssgaSphere.cxx           \
                          ssgaCelestialBody.cxx    \
                          ssgaCloudLayer.cxx       \
                          ssgaStars.cxx            \
                          ssgaSkyDome.cxx          \
                          ssgaSky.cxx              \
                          ssgaTeapot.cxx           \
                          ssgaBillboards.cxx

INCLUDES = -I$(top_srcdir)/src/sg -I$(top_srcdir)/src/ssg
INCLUDES += -I$(top_srcdir)/src/util

endif

EXTRA_DIST = ssgAux.dsp


