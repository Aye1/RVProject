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


#include "Fob.h"
#include <iostream>
#include <FobAlone.h>
#include <math.h>
#include <assert.h>

namespace arv {

/******************************************************************************************/

	Fob_HData toFobHemi(Fob::Hemisphere hemi) {
		switch (hemi) {
		case Fob::HEMI_FORWARD:
			return FORWARD;
			break;
		case Fob::HEMI_BACKWARD:
			return AFT;
			break;
		case Fob::HEMI_UPPER:
			return UPPER;
			break;
		case Fob::HEMI_LOWER:
			return LOWER;
			break;
		case Fob::HEMI_LEFT :
			return LEFT;
			break;
		case Fob::HEMI_RIGHT:
			return RIGHT;
			break;
		default:
			return FORWARD;
		}
	}

/******************************************************************************************/

	Fob::Fob(Hemisphere hemisphere, SerialPort port) {
		fob = new FobAlone(POSITION_QUATERNION, toFobHemi(hemisphere), getSystemName(port), CONTROL_SOFT, B9600);
	}

	void Fob::init(unsigned int number_of_birds) {
		if ( number_of_birds == 0 ){
			fob->InitAlone();
			positions_.resize(1);
			quaternions_.resize(1);
		}else{
			// Initialisation of the master
			fob->InitMaster(number_of_birds);
			// Initialisation of the slaves
			for ( unsigned int i = 2 ; i <= number_of_birds ; i++){
				fob->InitSlaveUnSeulRS232(i);
			}
			positions_.resize(number_of_birds);
			quaternions_.resize(number_of_birds);
		}
		this->number_of_birds_ = number_of_birds;
	}
	
	void Fob::close() {
	}

	void Fob::update() {
		// updates all the birds
		if ( number_of_birds_ == 0 ){
			// StandAlone mode
			update(0);
		}else{
			// Master/Slave mode
			for ( unsigned int i = 1 ; i <= number_of_birds_ ; i ++ ){
				update(i);
			}
		}
	}
	
	void Fob::update(unsigned int bird_number){
		float quat[4];
		float pos[3];
		if ( number_of_birds_ > 0 ){
			// Def prog
			assert((bird_number > 0) && (bird_number <= number_of_birds_));
			// Assume that GetPosQuaternion does only one
			// RS232 request. If it does not, the ToFbb
			// command is to launch before all the RS232
			// commands
			fob->ToFbb(bird_number);
		}else{
			// A bit tricky. When you are in StandAlone
			// Mode, the master (the only bird) bird is at
			// address 0, its number in this module is
			// 0. When you are in Master/Slave mode, its
			// address (and thus its number) is
			// 1. Therefore, to get the same number for
			// the master in the vectors of positions and
			// quaternions, I put its number to 1.
			bird_number++;
		}
		fob->GetPosQuaternion(pos, quat);
		 // Don't forget the birds range is from 1 to
		 // number_of_birds_ but Vector range is from 0 to
		 // number_of_birds_-1
		qglviewer::Vec& position = positions_[bird_number-1];
		qglviewer::Quaternion& quaternion = quaternions_[bird_number-1];
		for (int i=0; i<4; i++) quaternion[i] = quat[i];   // necessary because <ori> is a double[4]
		for (int i=0; i<3; i++) position[i] = pos[i];
	}
	
	qglviewer::Vec Fob::position(unsigned int bird_number) const {
		if ( number_of_birds_ > 0 ){
			assert((bird_number > 0) && ( bird_number <= number_of_birds_ ));
		}else{
			assert(bird_number==0);
			// Same trick as described in the
			// update(unsigned int) method to have the
			// range of birds into 0..number_of_birds_-1
			// whatever the situation is
			bird_number++;
		}
		return positions_[bird_number-1];
	}

	qglviewer::Quaternion Fob::orientation(unsigned int bird_number) const {
		if ( number_of_birds_ > 0 ){
			assert((bird_number > 0) && ( bird_number <= number_of_birds_ ));
		}else{
			assert(bird_number==0);
			// Same trick as described in the
			// update(unsigned int) method to have the
			// range of birds into 0..number_of_birds_-1
			// whatever the situation is
			bird_number++;
		}
		return quaternions_[bird_number-1];
	}

	qglviewer::Frame Fob::transform(unsigned int bird_number) const {
		if ( number_of_birds_ > 0 ){
			assert((bird_number > 0) && ( bird_number <= number_of_birds_ ));
		}else{
			assert(bird_number==0);
			// Same trick as described in the
			// update(unsigned int) method to have the
			// range of birds into 0..number_of_birds_-1
			// whatever the situation is
			bird_number++;
		}
		return qglviewer::Frame(positions_[bird_number-1], quaternions_[bird_number-1]);
	}
	
}

