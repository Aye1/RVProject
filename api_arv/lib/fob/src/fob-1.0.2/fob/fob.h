/*
-------------------------------------------------------------------------------
libfob - C++ interface to Ascension Technology Corporation's
         Flock of Birds position and orientation measurement system.
Copyright (C) 2002 Nathan Cournia

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
-------------------------------------------------------------------------------
*/

//FILE:         fob.h
//AUTHORS:      Nathan Cournia <nathan@cournia.com>
//              Eric Medlin <emedlin@vr.clemson.edu>

#ifndef COURNIA_FOB_H
#define COURNIA_FOB_H 1

#include <vector>
#include <string>
#include <cstdarg>
#include <pthread.h>
#include <termios.h>
#include <unistd.h>
#include "fob/quaternion.h"
#include "fob/vector.h"

#define _POSIX_SOURCE 1

//assume normal addressing mode (code actually checks for it)
//normal address mode only supports 14 birds
const int FOB_MAX_BIRDS = 14;

extern "C" {
	//!	Thread function to retreive information from a Ascension Flock of Birds.
	/*!
	 * \param flock A \a fob* pointing to the flock of birds this thread is getting data from.
	 * There is no reason for you to use the method, as it is used internally
	 * by \a fob.
	 */
	void* get_flock_data( void *flock );
}

//! Ascension Flock of Birds.
/*!
 * All orientation and position information is returned in units that
 * make sense to OpenGL's reference frame (x: screen right, 
 * y: screen up, z: out of screen).  Note that this is different
 * from the Flock of Birds' default reference frame (x: out of screen,
 * y: screen left, z: screen down).
 * Use this object as follows:
 * \code
 * //open the flock of birds that is on the 2nd serial port and
 * //with sensors in front of the main transmitter
 * //sleep about 500ms between configuration commands
 * fob flock( "/dev/ttyS1", fob::FORWARD, fob::FAST, 500 );
 *
 * //make sure that everything opened ok
 * if( !flock ) {
 *   //something went wrong
 *   std::cerr << "error: " << flock.get_error( ) << std::endl;
 *   return 1;
 * }
 *
 * //get a list of all birds in the flock
 * fob::bird_list birds = flock.get_birds( );
 *
 * //we want position, orientation, button information from all birds
 * for( unsigned int i = 0; i < birds.size( ); ++i ) {
 *   birds[ i ]->set_mode( fob::POSITION | fob::ORIENTATION | fob::BUTTONS );
 * }
 *
 * //set the flock flying
 * flock.fly( );
 *
 * //give the birds a little time to startup
 * sleep( 1 );
 *
 * //main loop
 * math::vector3 position;
 * math::quaternion orientation;
 * unsigned char buttons;
 * while( 1 ) {
 *   for( unsigned int i = 0; i < birds.size( ); ++i ) {
 *     //get latest info from birds
 *     birds[ i ]->get_position( position );
 *     birds[ i ]->get_quaternion( orientation );
 *     birds[ i ]->get_buttons( buttons );
 *   
 *     std::cout << "bird i: pos:     " << position << std::endl;
 *     std::cout << "bird i: quat:    " << orientation << std::endl;
 *     std::cout << "bird i: buttons: " 
 *       << ((buttons == fob::BUTTON_LEFT)   ? 1 | 0)
 *       << ((buttons == fob::BUTTON_MIDDLE) ? 1 | 0)
 *       << ((buttons == fob::BUTTON_LEFT)   ? 1 | 0) << std::endl;
 *   }
 * }
 *
 * //close the flock (optional)
 * flock.close( );
 * \endcode
 */
class fob
{
public:
	typedef unsigned char mode; //!< Desired output format for a bird.
	static const mode POSITION; //!< Request position information from bird.
	static const mode ORIENTATION; //!< Request orientation information from bird.
	static const mode BUTTONS; //!< Request button information from bird.

	static const unsigned char BUTTON_LEFT; //!< Left button mask.
	static const unsigned char BUTTON_MIDDLE; //!< Middle button mask.
	static const unsigned char BUTTON_RIGHT; //!< Right button mask.
	
	//! A Bird in a Flock of Birds
	class bird {
	 private:
		fob& m_flock; //!< Flock this bird belongs too.
		mutable pthread_mutex_t m_data_mutex; //!< Ensures exclusive access to data.
		math::vector3 m_position; //!< Position of the bird.
		math::vector3 m_fob_angles; //!< Orientation of the bird (in fob coords).
		math::vector3 m_angles; //!< Orientation of the bird.
		math::quaternion m_quaternion; //!< Orientation of the bird.
		math::matrix4 m_matrix; //!< Orientation/Position of the bird.
		math::quaternion m_rotation; //!< Rotation to apply to the bird's orientation.
		unsigned int m_address; //! Address of the bird.
		bool m_sensor; //!< If a sensor is attached.
		bool m_transmitter; //!< If a transmitter is attached.
		mode m_mode; //!< Data format we expect data from the flock to be in.
		unsigned char m_buttons; //!< Bitwised or'ed buttons pressed.
		bool m_ori_dirty; //!< \c true if new data has been retrieved.

		friend class fob;

		//! Locks this birds data.
		inline void lock_data( void ) const {
			pthread_mutex_lock( &m_data_mutex );
		}

		//! Unlocks this birds data.
		inline void unlock_data( void ) const {
			pthread_mutex_unlock( &m_data_mutex );
		}

		//! Decodes position/angle information from stream.
		bool unpack_pos_angle( unsigned char *buffer, int size );
		
		//! Decodes angle information from stream.
		bool unpack_angle( unsigned char *buffer, int size );

		//! Decodes position information from stream.
		bool unpack_pos( unsigned char *buffer, int size );

		//! Given a bird record from the flock stream, updates the bird.
		bool update( unsigned char *buffer, int size );

		//! Updates the birds orientation with the latest data from the flock.
		void update_orientation( void );

	 public:
		//! Default constructor.
		bird( fob& parent ): m_flock( parent ),
			m_address( 0 ), m_sensor( false ), 
			m_transmitter( false ), m_mode( 0x0 ), m_buttons( 0x0 ),
			m_ori_dirty( false )
		{
			pthread_mutex_init( &m_data_mutex, NULL );
		}

		//! Copy constructor.
		bird( const bird& cpy ): m_flock( cpy.m_flock ) {
			//initialize mutex
			pthread_mutex_init( &m_data_mutex, NULL );
			
			//lock data 
			cpy.lock_data( );

			//copy data
			m_position = cpy.m_position;
			m_fob_angles = cpy.m_fob_angles;
			m_angles = cpy.m_angles;
			m_quaternion = cpy.m_quaternion;
			m_matrix = cpy.m_matrix;
			m_address = cpy.m_address;
			m_sensor = cpy.m_sensor;
			m_transmitter = cpy.m_transmitter;
			m_mode = cpy.m_mode;
			m_buttons = cpy.m_buttons;
			m_ori_dirty = cpy.m_ori_dirty;

			//unlock data
			cpy.unlock_data( );
		}

		//! Default destructor.
		~bird( void ) {
			pthread_mutex_destroy( &m_data_mutex );
		}

		//! Assignment operator.
		inline const bird& operator= ( const bird& rhs ) {
			//account for self assignment (which would cause a deadlock)
			if( &rhs == this ) return *this;

			//lock data (FIXME possible deadlock here)
			lock_data( );
			rhs.lock_data( );

			//copy data
			m_flock = rhs.m_flock;
			m_position = rhs.m_position;
			m_fob_angles = rhs.m_fob_angles;
			m_angles = rhs.m_angles;
			m_quaternion = rhs.m_quaternion;
			m_matrix = rhs.m_matrix;
			m_address = rhs.m_address;
			m_sensor = rhs.m_sensor;
			m_transmitter = rhs.m_transmitter;
			m_mode = rhs.m_mode;
			m_buttons = rhs.m_buttons;
			m_ori_dirty = rhs.m_ori_dirty;

			//unlock data
			rhs.unlock_data( );
			unlock_data( );

			//return reference to self
			return *this;
		}

		//! Gets the bird's position.
		inline void get_position( math::vector3& output ) const {
			lock_data( );
			if( m_ori_dirty ) {
				output = m_position;
			}
			unlock_data( );
		}

		//! Gets the bird's position.
		inline void get_position( real_t output[ 3 ] ) const {
			lock_data( );
			memcpy( output, static_cast<const real_t*>( m_position ), sizeof( real_t ) * 3 );
			unlock_data( );
		}

		//! Gets the bird's angles (in degress).
		/*!
		 * Rotations are relative to the fixed global coordinate frame.
		 */
		inline void get_angles( math::vector3& output ) {
			lock_data( );
			if( m_ori_dirty ) {
				update_orientation( );
			}
			output = m_angles;
			unlock_data( );
		}

		//! Gets the bird's angles (in degress).
		/*!
		 * Rotations are relative to the fixed global coordinate frame.
		 */
		inline void get_angles( real_t output[ 3 ] ) {
			lock_data( );
			if( m_ori_dirty ) {
				update_orientation( );
			}
			memcpy( output, static_cast<const real_t*>( m_angles ), sizeof( real_t ) * 3 );
			unlock_data( );
		}

		//! Gets the bird's orientation.
		inline void get_quaternion( math::quaternion& output ) {
			lock_data( );
			if( m_ori_dirty ) {
				update_orientation( );
			}
			output = m_quaternion;
			unlock_data( );
		}

		//! Gets the bird's orientation.
		/*
		 * Returns the quaternion in x y z w order.
		 */
		inline void get_quaternion( real_t output[ 4 ] ) {
			lock_data( );
			if( m_ori_dirty ) {
				update_orientation( );
			}
			memcpy( output, static_cast<const real_t*>( m_quaternion.vec( ) ), sizeof( real_t ) * 3 );
			output[ 3 ] = m_quaternion.w( );
			unlock_data( );
		}
		
		//! Gets a 4x4 matrix describing the bird's orientation/position.
		/*!
		 * The returned matrix is in row major format.
		 */
		inline void get_matrix( math::matrix4& output ) {
			lock_data( );
			if( m_ori_dirty ) {
				update_orientation( );
			}
			output = m_matrix;
			unlock_data( );
		}

		//! Gets a 4x4 matrix describing the bird's orientation/position.
		/*!
		 * The returned matrix is in row major format.
		 */
		inline void get_matrix( real_t output[ 16 ] ) {
			lock_data( );
			if( m_ori_dirty ) {
				update_orientation( );
			}
			memcpy( output, static_cast<const real_t*>( m_matrix ), sizeof( real_t ) * 16 );
			unlock_data( );
		}
		
		//! Sets what type of data we will get back from the flock for this bird.
		/*!
		 * \param mask Bitwised or'ed \c fob::mode telling what data we want back from the flock.  Options are \c fob::POSITION, \c fob::ORIENTATION, and \c fob::BUTTONS.
		 * \returns \c true on success.  \c false on error. Flock that bird is apart of contains error message on error.
		 */
		bool set_mode( fob::mode mask );

		//! Applies the given rotation to the each orientation update.
		void set_rotation( const math::quaternion& rot );

		//! Returns the birds bitwise or'ed button state.
		/*!
		 * To determine what buttons are presses, bitwise or
		 * with the following variables:
		 * \code
		 * if( buttons | fob::BUTTON_LEFT ) //left pressed
		 * if( buttons | fob::BUTTON_MIDDLE ) //middle pressed
		 * if( buttons | fob::BUTTON_RIGHT ) //right pressed
		 * \endcode
		 */
		inline unsigned char get_buttons( void ) const {
			return m_buttons;
		}
	};

	//! A list of birds.
	typedef std::vector<fob::bird*> bird_list;
	
	//! Hemisphere in which all sensors are operating.
	enum hemisphere {
		AFT,
		FORWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	//! Serial port speed.
	enum port_speed {
		FAST,
		SLOW
	};

	//! Serverity of hardware error.
	enum error_level {
		WARNING1,
		WARNING2,
		FATAL
	};

	//! Describes an internal flock hardware error.
	struct error_t {
		const char *msg;   //!< Descriptive message describing the error.
		error_level level; //!< Severity of the error.
	};

 private:
	bool m_error; //!< \c true if in error state.
	std::string m_error_msg; //!< Error message.

	pthread_t m_flock_thread; //!< Thread that downloads streamed data from flock.
	bird_list m_birds; //!< List of known birds.
	bool m_open; //!< \c true if the flock of birds device has been opened.
	int m_device; //!< File handle to fob device (serial port).
	struct termios m_save_tio; //!< Old saved for settings for fob serial port.
	unsigned int m_master; //!< FBB address of master bird ([1,14] probably 1).
	bool m_fly; //!< \c true if in fly mode.
	bool m_group; //!< \c true in group mode.
	pthread_mutex_t m_mutex; //!< Private data access mutex.
	unsigned char m_save; //!< Saved byte from input stream.
	hemisphere m_hemisphere;
	unsigned long m_sleep; //!< Time to sleep between fob commands.
	error_level m_min_error_level; //!< Don't report errors below this error level.
	
	//! Clears internal error state.
	inline void clear_error( void ) {
		m_error = false;
		m_error_msg = "none";
	}

	//! Sets internal error state.
	inline void set_error( const char *fmt, ... ) {
		char buffer[ 1024 ];
		va_list ap;
		va_start( ap, fmt );
		vsnprintf( buffer, 1024, fmt, ap );
		va_end( ap );
		m_error = true;
		m_error_msg = buffer;
	}

	//! Locks private data.
	inline void lock( void ) {
		pthread_mutex_lock( &m_mutex );
	}

	//! Unlocks private data.
	inline void unlock( void ) {
		pthread_mutex_unlock( &m_mutex );
	}

	//! Thread data retrieval function.
	friend void* get_flock_data( void *flock );

	//! Flock of Birds command.
	struct command {
		unsigned char cmd; //!< Command character.
		int option_bytes; //!< Size (in b1ytes) this command's arguments total.
	};
	
	//! Flock of Birds examine command.
	struct examine_option {
		unsigned char param; //!< Examine parameter.
		int reply_bytes; //!< Expected size of reply in bytes.
	};

	//! Flock of Birds change command.
	struct change_option {
		unsigned char param; //!< Change parameter.
		int option_bytes; //!< Size (in b1ytes) this command's arguments total.
	};

	//! Flush (clears) all input/output buffers to the serial port where the Flock of Birds is connected.
	void clear_device( void );

	//! Reads data from the serial port in an interrupt safe manner.
	int read( unsigned char *output, int bytes );

	//! Sends a 1 character command.
	bool send_cmd( unsigned char cmd );

	//! Sends a command that requires arguments.
	/*!
	 * \param cmd Command to send.
	 * \param Array that holds command arguments.
	 * \return \c true on success.
	 */
	bool send_cmd( fob::command cmd, unsigned char *options );

	//! Sends an examine command.
	/*!
	 * \param param Examine command to send.
	 * \param output Array that will be loaded with the examine reply.
	 * \returns \c true on success.
	 */
	bool examine( examine_option param, unsigned char *output );

	//! Sends a change command.
	/*!
	 * \param param Change command to send.
	 * \param option Array that holds command arguments.
	 * \returns \c true on success.
	 */
	bool change( change_option param, unsigned char *option );

	//! Makes sure the addressing mode is valid.
	/*!
	 * The Flock of Birds can operate in one of 3 addressing modes.
	 * This method makes sure that the flock is in the "normal"
	 * addressing mode (an assumption this class is based around).
	 * \returns \c true if the addressing mode is valid.
	 */
	bool check_address_mode( void );

	//! Loads the class with sensor/transmitter information.
	/*!
	 * \returns \c false if an error occurred.
	 */
	bool load_flock_status( void );

	//! Automatically configures the flock for operation.
	/*!
	 * \returns \c false if an error occurred.
	 */
	bool auto_configure( unsigned int num_birds );

	//! Sets the voltage of the RTS (Ready to Send) pin.
	/*!
	 * Assumes pin 8 of the RS232 cable is attached to the flock.
	 * \param high \c true if the pin should be set to high voltage, \c false otherwise.
	 * \returns \c false if an error occurred.
	 */
	bool set_rts( bool high );

	//! Tells the flock that the given bird will be operating in the given hemisphere.
	bool set_hemisphere( fob::bird& b, fob::hemisphere hemi );
	
	//! Perform a hard reset of the flock of birds.
	/*!
	 * Assumes pin 8 of the RS232 cable is attached to the flock.
	 * \returns \c false if an error occurred.
	 */
	bool reset( void );

	//! Check the flock for an error.
	/*!
	 * If the flock is in an error state.  This object will go into
	 * an error state.  The error message can then be checked with
	 * fob::get_error.
	 * \returns \c true if an error occurred.
	 */
	bool check_error( void );

	//! Selects the given bird as the current bird.
	bool select_bird( bird& b );

	//! Selects the bird at the given bird address as the current bird.
	bool select_bird( int bird_addr );

	//! Updates the bird list with new data from flock.
	/*!
	 * This method probably should not be called by the programmer.
	 * It is used mainly internally by the flock's internal thread.
	 */
	bool update( void );

	//! Sleep between flock commands.
	void cmd_sleep( void ) const {
		cmd_sleep( m_sleep );
	}

	//! Sleep between flock commands.
	/*!
	 * \param us Time to sleep in microseconds.
	 */
	void cmd_sleep( unsigned long us ) const {
		//if sleep time is 0, don't call sleep (since it may
		//give up the processor)
		if( us > 1 ) {
			usleep( us );
		}
	}

	//see fob.cpp for definitions
	static const unsigned char STREAM;
	static const unsigned char POINT;
	static const unsigned char SLEEP;
	static const unsigned char EXAMINE;
	static const unsigned char CHANGE;
	static const unsigned char MODE_POS;
	static const unsigned char MODE_ANG;
	static const unsigned char MODE_POS_ANG;
	static const unsigned char MODE_POS_MAT;
	static const unsigned char MODE_POS_QUAT;
	static const unsigned char MODE_QUAT;
	static const unsigned char FBB_RESET;
	static const unsigned char RUN;
	static const command HEMISPHERE;
	static const command BUTTON_MODE;
	static const command REF_FRAME_1;
	static const command REF_FRAME_2;
	static const examine_option BIRD_STATUS;
	static const examine_option ERROR_CODE;
	static const examine_option MODEL_ID;
	static const examine_option REF_FRAME_1_EXAMINE;
	static const examine_option ADDR_MODE;
	static const examine_option FLOCK_STATUS;
	static const change_option REF_FRAME_1_ENABLE;
	static const change_option GROUP;
	static const change_option FBB_AUTO_CONFIG;
	static const float SCALE;
	static const float ANGLE_SCALE;
	
public:
	//! Constructor
	/*!
	 * Creates an \a fob object.  It does not open the flock.  To
	 * open the flock, call fob::open.
	 */
	fob( void );

	//! Constructor with options.
	/*!
	 * Creates an \a fob object.  Opens the flock.
	 * \param device_name Serial port the flock is connected too.
	 * \param hemi Hemisphere in which all birds of the flock will operate.
	 * \param speed Speed at which the host system will communicate with the flock hardware.
	 *              Use fob::SLOW for older flock hardware.
	 * \param sleep_ms Amount of time to sleep between sending commands to the flock hardware (in ms).
	 */
	fob( const std::string& device_name, fob::hemisphere hemi,
	  port_speed speed = FAST, unsigned long sleep_ms = 500 );

	//! Destructor
	/*!
	 * Closes the flock of birds and reclaims any memory used by
	 * the \a fob object.
	 */
	~fob( void );

	//! Opens the flock of birds.
	/* \param device_name Serial port the flock is connected too.
	 * \param hemi Hemisphere in which all birds of the flock will operate.
	 * \param speed Speed at which the host system will communicate with the flock hardware.
	 *              Use fob::SLOW for older flock hardware.
	 * \param sleep_ms Amount of time to sleep between sending commands to the flock hardware (in ms).
	 */
	const fob& open( const std::string& device_name, 
		fob::hemisphere hemisphere, port_speed speed = FAST,
		unsigned long sleep_ms = 500 );

	//! Closes the flock of birds.
	bool close( void );

	//! Returns a list of birds in the flock.
	inline fob::bird_list& get_birds( void ) {
		return m_birds;
	}

	//! Sets the birds flying.
	bool fly( void );

	//! Print out status of each bird.
	bool print_bird_status( void );

	//!Return \c false if flock is in an error state.
	inline operator bool ( void ) const {
		return !m_error;
	}

	//! Returns a string describing the current error.
	inline const std::string& get_error( void ) const {
		return m_error_msg;
	}

	//! Sets the amount of time to sleep between flock commands (in ms).
	/*!
	 * Sending data to the flock to fast results in placing the
	 * flock into an unstable state.  One can use this method
	 * to set a small amount of time the library should sleep
	 * when sending data to the flock.  Note that this time only
	 * effects the flock startup time.  It does not effect the 
	 * rate at which flock data is retrieved.
	 * \param ms Time to sleep in milliseconds.
	 */
	inline void set_sleep( unsigned long ms ) {
		//convert from ms to us
		m_sleep = ms * 1000;
	}

	//! Returns the amount of time to sleep (in ms) between flock commands.
	inline unsigned long get_sleep( void ) const {
		return m_sleep / 1000;
	}

	//! Sets the error level at which flock hardware errors will cause the \a fob object enter an error state.
	/*!
	 * \param lvl Error levels below this parameter will not be reported.
	 */
	inline void set_error_level( error_level lvl ) {
		m_min_error_level = lvl;
	}
};

#endif
