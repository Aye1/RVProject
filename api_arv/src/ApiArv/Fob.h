/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 *  remarks :
 *      Implementation of the Flock of Birds based on previous Flock Of Birds drivers by Raphael Grasset.
 *      This interface restricts access to a set of minimal functionalities.
 *      It runs only on point mode, stream mode is more difficult to manage when there is application errors or too much delay to get values.
 *      Tested with 9600 baud : demonstrates high speed measurement.
 *      It actually drives with software synchronisation mode (XON/XOFF) with a 3-pin serial cable. It plugs itself into 16550A UART serial port of the PC.
 *      Default coordinate system changed to match the better suited OpenGL coordinate system.   
 */


#ifndef __ARV_FOB__
#define __ARV_FOB__

#include <QGLViewer/vec.h>
#include <QGLViewer/quaternion.h>
#include <QGLViewer/frame.h>
#include <vector>
#include "SerialPort.h"

class FobAlone;

namespace arv {

	/** Interface for the Flock of Birds tracker.
	 * The Flock of Birds (FOB) delivers 6DOF data from a tracker (their) with real-time rates.
	 * The bird needs a specific hemisphere to operate, and it needs to be positionned
	 * in this hemisphere at startup to function correctly.
	 */
	class Fob {
        public:
		/** This type represents hemispheres of the FOB magnetic transmitter. */
		enum Hemisphere { HEMI_FORWARD, HEMI_BACKWARD, HEMI_UPPER, HEMI_LOWER, HEMI_LEFT, HEMI_RIGHT };
        
        public:
		Fob(Hemisphere hemisphere = HEMI_UPPER, SerialPort port = PORT0);
                /**< Create a new instance.
		   @param hemisphere the hemisphere where the bird will fly.
		   @param port the serial port into which the FOB is plugged.
		*/
            
		void init(unsigned int number_of_birds=0);
                /**< Initializes communication with the FOB.
		   
		   \param number_of_birds: If 0, run in StandAlone
		   mode, else run in Master/Slaves mode with only
		   communication with the master via RS232 port (thus,
		   number_of_birds-1 slaves).
		*/
            
		void close();
                /**< Closes communication with the FOB.
                 */
            
		void update();
                /**< Updates birds's state from their physical positions and orientations.
                 */

		void update(unsigned int bird_number);
		/**< Update the bird of number bird_number, starting
		 * from 1 to number_of_birds (= the number given in the init)
		 */

		qglviewer::Vec position(unsigned int bird_number=0) const;
                /**< Gets the bird's position.
		   @return the position of the bird during last update.
		*/

		qglviewer::Quaternion orientation(unsigned int bird_number=0) const;
                /**< Gets the bird's orientation.
		   @return the orientation of the bird during last update.
		*/

		qglviewer::Frame transform(unsigned int bird_number=0) const;
                /**< Gets the bird's transform.
		   @return the transfomation defined by position and orientation of the bird during last update.
		*/
            
        private:
		FobAlone* fob;
		std::vector<qglviewer::Vec>        positions_;   // the bird's position
		std::vector<qglviewer::Quaternion> quaternions_; // the bird's orientation
		unsigned int number_of_birds_;                   // The number of birds used
        };
    
}

#endif

