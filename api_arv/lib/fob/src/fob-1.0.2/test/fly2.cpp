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

//FILE:         fly2.cpp
//AUTHOR:       Nathan Cournia <nathan@cournia.com>

#include <iostream>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "camera.h"
#include "fob/fob.h"
#include "args.h"

#define ENABLE_FBB 1

const unsigned int MAX_ENTS = 14;

void render_axis( float len );
void render_buttons( bool x, bool y, bool z, float len );

///////////////////////////////////////////////////////////////////////////////
struct entity {
	math::quaternion orientation;
	math::vector3 position;
	math::matrix4 matrix;
	unsigned char buttons;

	void render( void );
};




///////////////////////////////////////////////////////////////////////////////
void
entity::render( void )
{
	//save modelview
	glPushMatrix( );

	//translate and rotate to bird position
	matrix.transpose( );
	glMultMatrixf( matrix );
	
	//render the mesh
	render_axis( 10 );

	//render buttons
	render_buttons( 
		(buttons == fob::BUTTON_LEFT), 
		(buttons == fob::BUTTON_MIDDLE), 
		(buttons == fob::BUTTON_RIGHT), 
		10 
	);

	//back to the old matrix
	glPopMatrix( );
}



//////////////////////////////////////////////////////////////////////////
//globals!  yeah!
entity entities[ MAX_ENTS ];
camera cam;
fob flock;
unsigned int num_birds;
fob::bird_list *p_birds;
GLUquadricObj *sphere = 0;



//////////////////////////////////////////////////////////////////////////
void 
render_axis( float len )
{
	glLineWidth( 2.0 );
	glBegin( GL_LINES );
		//+x bright red
		glColor3f( 1.0, 0.0, 0.0 );
		glVertex3f( len, 0.0, 0.0);
		glVertex3f( 0.0, 0.0, 0.0 );

		//+y bright green
		glColor3f( 0.0, 1.0, 0.0 );
		glVertex3f( 0.0, len, 0.0 );
		glVertex3f( 0.0, 0.0, 0.0 );

		//+z bright blue
		glColor3f( 0.0, 0.0, 1.0 );
		glVertex3f( 0.0, 0.0, len );
		glVertex3f( 0.0, 0.0, 0.0 );
	glEnd( );
}



//////////////////////////////////////////////////////////////////////////
void 
render_buttons( bool x, bool y, bool z, float len )
{
	if( x ) {
		glColor3f( 1.0, 0.0, 0.0 );
		glTranslatef( len, 0.0, 0.0 );
		gluSphere( sphere, len / 10.0, 7, 7 );
		glTranslatef( -len, 0.0, 0.0 );
	}
	if( y ) {
		glColor3f( 0.0, 1.0, 0.0 );
		glTranslatef( 0.0, len, 0.0 );
		gluSphere( sphere, len / 10.0, 7, 7 );
		glTranslatef( 0.0, -len, 0.0 );
	}
	if( z ) {
		glColor3f( 0.0, 0.0, 1.0 );
		glTranslatef( 0.0, 0.0, len );
		gluSphere( sphere, len / 10.0, 7, 7 );
		glTranslatef( 0.0, 0.0, -len );
	}
}



///////////////////////////////////////////////////////////////////////////////
void
init_gl( void )
{
	//opengl options
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_LIGHTING );
}



///////////////////////////////////////////////////////////////////////////////
void
handle_keyboard( unsigned char key, int x, int y )
{
	if( key == 27 ) { //ESC pressed
		exit( 0 );
	}
}



///////////////////////////////////////////////////////////////////////////////
void
handle_resize( int w, int h )
{
	//prevent divide by zero
	if( h == 0 ) h = 0;

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	glViewport( 0, 0, w, h );
	gluPerspective( 45.0, static_cast<float>( w ) / h, 1.0, 1000.0 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
}



///////////////////////////////////////////////////////////////////////////////
void
handle_exit( void )
{
	if( sphere ) {
		gluDeleteQuadric( sphere );
		sphere = 0;
	}
	flock.close( );
}



///////////////////////////////////////////////////////////////////////////////
void
render( void )
{
		//clear gl buffers
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		//reset modelview
		glLoadIdentity( );

		//view from camera
		cam.view( );

		//render a reference frame
		render_axis( 100000 );

#if ENABLE_FBB
		//update entity positions
		for( unsigned int i = 0; i < num_birds; ++i ) {
			//(*p_birds)[ i ]->get_position( entities[ i ].position );
			//(*p_birds)[ i ]->get_quaternion( entities[ i ].orientation );
			(*p_birds)[ i ]->get_matrix( entities[ i ].matrix );
			entities[ i ].buttons = (*p_birds)[ i ]->get_buttons( );
		}
#endif

		//render entities
		for( unsigned int i = 0; i < num_birds; ++i ) {
			entities[ i ].render( );
		}

		//done
		glutSwapBuffers( );
}



///////////////////////////////////////////////////////////////////////////////
int 
main( int argc, char *argv[ ] )
{
	cmd_args args;
	if( !args.parse( argc, (const char**)argv ) ) {
		return 1;
	}

	//setup glut
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
	glutInitWindowSize( 640, 480 );
	glutCreateWindow( "Flock of Birds Demo" );

	//setup gl
	init_gl( );

	//set callbacks
	glutKeyboardFunc( handle_keyboard );
	glutReshapeFunc( handle_resize );
	glutDisplayFunc( render );
	glutIdleFunc( render );

#if ENABLE_FBB
	fob::hemisphere hemisphere = fob::DOWN;
	fob::port_speed speed = args.speed;
	
	//talk to flock
	flock.open( args.serial, hemisphere, speed, args.sleep_ms );
	if( !flock ) {
		std::cerr << "fatal: " << flock.get_error( ) << std::endl;
		return 1;
	}

	//get a list of birds connected to the machine
	fob::bird_list& birds = flock.get_birds( );
	p_birds = &flock.get_birds( );
	num_birds = birds.size( );
	if( num_birds > MAX_ENTS ) num_birds = MAX_ENTS;
#else
	unsigned int num_birds = 1;
#endif
	
#if ENABLE_FBB
	//for each bird, set that we want position and orientation
	for( unsigned int i = 0; i < birds.size( ); ++i ) {
		if( !birds[ i ]->set_mode( fob::POSITION | fob::ORIENTATION | fob::BUTTONS ) ) {
			std::cerr << "fatal: " << flock.get_error( ) << std::endl;
			return 1;
		}
	}
#endif

	//setup the camera
	cam.set_position( 0.0, 100.0, 0.0 );
	cam.set_look_right( math::vector3( 0.0, 0.0, 0.0 ), math::vector3::X_AXIS );
	
#if ENABLE_FBB
	//set the flock flying
	flock.fly( );
	atexit( handle_exit );

	//let the bird start up . . .
	sleep( 1 );
#endif

	//create the button sphere
	sphere = gluNewQuadric( );

	//start glut
	glutMainLoop( );

	//no errors
	return 0;
}
