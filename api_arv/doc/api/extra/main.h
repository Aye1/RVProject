/** @mainpage Introduction

@section organisation Organisation of Api ARV

In the following documentation \c ARV_ROOT will denote the root of this api (usually \c /usr/local/api_arv).
It is organised as follows :
- \c ARV_ROOT/data : place for common data used by the api
- \c ARV_ROOT/doc : general place for documentation.
- \c ARV_ROOT/doc/api : contains this documentation and the doxygen confiuration file to generate it.
- \c ARV_ROOT/examples : contains projects examples using this api.
- \c ARV_ROOT/lib : contains external libs used by classes of the api
- \c ARV_ROOT/qmake : contains \c qmake configuration files (\c .prf) for this api
- \c ARV_ROOT/src : contains the source files of the api.
- \c ARV_ROOT/tests : contains tests for classes of this api.
- \c ARV_ROOT/tools : contains various useful tools.


@section compilation1 Api ARV projects compilation
Using qmake is the simplest way to compile a project making use of ApiArv classes.
There is a template project file in \c ARV_ROOT/examples.

A typical qmake project configuration file (.pro) looks like this :

@verbatim
TARGET   = Prog                 # name of the produced executable
TEMPLATE = app                  # make an application, not a library
CONFIG *= warn_on debug         # to get warnings and debug info
CONFIG *= arv                   # necessary to use ApiArv classes
CONFIG *= qglviewer glut        # possibly use other external libs

ARV = camera tracker sound      # tells which ApiArv classes are needed

HEADERS = sources//**.h           # all the C++ headers of the project
SOURCES = sources//**.cpp         # all the C++ sources of the project

OBJECTS_DIR = .obj              # the directory where will be placed object (.o) files.
@endverbatim

Once this project file is created just run the following commands in a shell, in the directory containing the \c .pro file :
@code
> qmake
> make
@endcode

The program should be compiled.


@section compilation2 How does that work?

\c qmake works with configuration files indentified by a \c .prf extension.

For each token (e.g. \c arv) put in the \c CONFIG variable,
\c qmake will search for a corresponding \c .prf file (e.g. \c arv.prf) in standard Qt locations and also in directories listed
in the \c QMAKEFEATURES environment variable (or the corresponding Qt "property variable" whose value can be queried using \c qmake \c -query or set using \c qmake \c -set \c QMAKEFEATURES \c ...).

The directory \c ARV_ROOT/qmake contains useful \c .prf files for ApiArv projects configuration.

To use any Api ARV class, two steps are needed :
    - tell that some \c arv classes are going to be used : \c CONFIG \c += \c arv
    - specify with variable \c ARV which ApiArv classes are going to be used, e.g. : \c ARV \c += \c camera \c glove \c tracker

Simply adding these two lines to your project file makes automatic and transparent the necessary includes and dependencies resolution for ApiArv classes.

Variable \c ARV understands the following tokens :
    - \c camera
    - \c fob
    - \c glove
    - \c image
    - \c keyboard
    - \c microphone
    - \c motorized_camera
    - \c mouse
    - \c mutex
    - \c serial_port
    - \c sound3d
    - \c sound
    - \c tcp_socket
    - \c thread
    - \c tracker
    - \c udp_socket
    - \c yoke


@section code ApiArv headers inclusion.

To avoid any name collisions with other's libs headers, all ApiArv files are located in a \c ApiArv subdirectory. Only the parent of this directory is put in the \c INCLUDEPATH,
so that it makes necessary to explicitly specify the full names to include ApiArv headers, e.g.:
@code
#include <ApiArv/Camera.h>
@endcode
Furthermore, all ApiArv classes belong to the \c arv namespace.


*/

