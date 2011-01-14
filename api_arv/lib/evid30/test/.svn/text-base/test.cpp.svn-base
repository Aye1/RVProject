#include <cc_evid.hpp>
#include <iostream>

void showHelp( void );

int main ( int argc, void* argv[] ) {
    int serial_line = -1;

    if ( argc == 2 )
        serial_line = atoi ( (char*)argv[1] );
    else
        for (;;) {
            std::cout << "enter serial line number ";
            std::cin >> serial_line;

            if ( ( serial_line > 2 ) || ( serial_line < 1 ) )
                std::cout << "number should be either 1 or 2" << std::endl;
            else
                break;
            }

    //
    //  instantiate camera object
    //
    CEVID30 cam;

    //
    //
    //  initialize camera object
    //
    //  if there is an error, check cables or enter another serial line number
    //
    if ( cam.Init(serial_line) ) {
        std::cerr << "error while initializing camera" << std::endl;
        std::cerr << "check cable or enter another serial line number" << std::endl;
        exit ( EXIT_FAILURE );
        }

    char command = 0;
    long position = 0;

    //
    //  now the camera can be used
    //
    while ( command != 'x' ) {
        std::cout << "Enter command [h for help] : " << std::flush;
        std::cin >> command;
        //    std::cout << "command = " << command << std::endl;
        switch ( command ) {
            case 'h' :
                showHelp();
                break;
            case 's' :
                cam.stopZoom();
                cam.stopPanTilt();
                break;
            case 'i' :
                cam.startSlowTeleZoom();
                break;
            case 'o' :
                cam.startSlowWideZoom();
                break;
            case 'r' :
                cam.startPanRight();
                break;
            case 'l' :
                cam.startPanLeft();
                break;
            case 'u' :
                cam.startTiltUp();
                break;
            case 'd' :
                cam.startTiltDown();
                break;
            case 'p' :
                cam.reqPanAbsPos(position);
                std::cout << "Current Pan Position : " << position << std::endl;
                break;
            case 't' :
                cam.reqTiltAbsPos(position);
                std::cout << "Current Tilt Position : " << position << std::endl;
                break;
            case 'z' :
                cam.reqAbsZoomPos(position);
                std::cout << "Current Zoom Position : " << position << std::endl;
                break;
            case 'g' :
                std::cin >> position;
                cam.FastPanAbsPosition ( position );
                break;
            case 'f' :
                std::cin >> position;
                cam.FastTiltAbsPosition ( position );
                break;
            case 'm' :
                std::cin >> position;
                cam.FastSetAbsZoomPos ( position );
                break;
            case 'a' :
                cam.turnPowerOn();
                break;
            case 'q' :
                cam.turnPowerOff();
                break;
            case 'b' :
                cam.backLightOn();
                break;
            case 'v' :
                cam.backLightOff();
                break;
            default :
                std::cout << "ERROR! Unknown command!" << std::endl;
                break;
            };
        }

    std::cout << "program ended normally" << std::endl;
    return EXIT_SUCCESS;
    }


void showHelp ( void ) {
    std::cout << "\n\th\t:\tthis help function" << std::endl;
    std::cout << "\tx\t:\texit program" << std::endl;
    std::cout << "\ts\t:\tstop moving (pan, tilt, zoom)" << std::endl;
    std::cout << "\ti\t:\tstart zooming in" << std::endl;
    std::cout << "\to\t:\tstart zooming out" << std::endl;
    std::cout << "\tr\t:\tstart panning right" << std::endl;
    std::cout << "\tl\t:\tstart panning left" << std::endl;
    std::cout << "\tu\t:\tstart tilting up" << std::endl;
    std::cout << "\td\t:\tstart tilting down" << std::endl;
    std::cout << "\tp\t:\tshow current pan position" << std::endl;
    std::cout << "\tt\t:\tshow current tilt position" << std::endl;
    std::cout << "\tz\t:\tshow current zoom position" << std::endl;
    std::cout << "\tg 34\t:\tpan to position 34 [-858...861]" << std::endl;
    std::cout << "\tf 255\t:\ttilt to position 255 [-280...283]" << std::endl;
    std::cout << "\tm 200\t:\tzoom to position 200 [0...1023]" << std::endl;
    std::cout << "\ta\t:\tturn power on" << std::endl;
    std::cout << "\tq\t:\tturn power off" << std::endl;
    std::cout << "\tb\t:\tturn backlight on" << std::endl;
    std::cout << "\tv\t:\tturn backlight off" << std::endl;
    std::cout << std::endl;
    }



