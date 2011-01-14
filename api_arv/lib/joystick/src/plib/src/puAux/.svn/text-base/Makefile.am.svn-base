if BUILD_PUAUX

lib_LIBRARIES = libplibpuaux.a

include_HEADERS = puAux.h

libplibpuaux_a_SOURCES = puAux.cxx                  \
                         puAuxBiSlider.cxx          \
                         puAuxBiSliderWithEnds.cxx  \
                         puAuxComboBox.cxx          \
                         puAuxCompass.cxx           \
                         puAuxFileSelector.cxx      \
                         puAuxLargeInput.cxx        \
                         puAuxScrollBar.cxx         \
                         puAuxSelectBox.cxx         \
                         puAuxSliderWithInput.cxx   \
                         puAuxSpinBox.cxx           \
                         puAuxTriSlider.cxx         \
                         puAuxVerticalMenu.cxx

INCLUDES = -I$(top_srcdir)/src/sg  \
           -I$(top_srcdir)/src/pui \
           -I$(top_srcdir)/src/fnt \
           -I$(top_srcdir)/src/util

endif

EXTRA_DIST = puAux.dsp


