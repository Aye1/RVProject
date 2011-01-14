///////////////////////////////////////////////////////////////////////////////
// File name : RS232Interface.h
//
// Creation : 1999/03/16
// Version : 1.0
//
// Author : Raphael Grasset
// email : Raphael.Grasset@imag.fr
//
// Purpose : defini une classe pour communiquer avec le fob sur l'interface RS232
//elle gere principalement la communication avec le port serie. Le flot de controle est
//effectue de facon HARDWARE.
//l'ouverture d'un port sera donc toujours sur un /dev/ttyf*
// Distribution :
//
// Use :
//	type de fonctionnement du controle de flux: 
// XON/XOFF (3 fils) ou  HARD (7 fils)
//
// Todo :
//	O	
//	/
//	X
//
// History :
//	1999/03/16 : Mr Grasset : Create of the file
///////////////////////////////////////////////////////////////////////////////

#if defined RS232Interface_CYCLE
#error Header cyclic inclusion detected in RS232Interface.h
#else // defined RS232Interface_CYCLE
#define RS232Interface_CYCLE

#if !defined RS232Interface_h
#define RS232Interface_h

///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <termios.h>  // terminal (comm port) setting
#include <fcntl.h>    // file control
#include <unistd.h>   // sleep
#include <time.h>
#include <ctype.h> // isprint
#include <stdlib.h>
#include <string.h>
//#include <iostream>
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////
// class RS232Interface
///////////////////////////////////////////////////////////////////////////////


enum RS232_Mode {CONTROL_HARD,CONTROL_SOFT};


class RS232Interface {

public:

//constructeur
RS232Interface(const char *comm="/dev/ttyf2",long baudspeed=B9600,RS232_Mode
mode=CONTROL_HARD);

//destructeur
~RS232Interface() {}


//methodes

/* connect
etabli la connection sur le port.
pas de parametres en entree, ni en sortie.
*/
void connect(); 

/* disconnect 
ferme la connection sur le port.
*/
void disconnect();

/*clearBuffer
nettoie le buffer d'entree et de sortie.
*/
void clearBuffer();

/*put
envoie des bytes au fob.
Si on ne peut envoyer tous les bytes on renvoi une erreur.
entree:	data:donnees a envoyes.
	nb_bytes:nombre de bytes a envoyes.

retour:1 si reussite 0 sinon.
*/
int put(unsigned char *data,int nb_bytes);

/*put_c
envoie une commande (un byte) au fob.
Si on ne peut envoyer la commande on renvoi une erreur.
entree:	data:donnees a envoyes.
	nb_bytes:nombre de bytes a envoyes.

retour:1 si reussite 0 sinon.
*/
int put_c(unsigned char data);

/*get
recuperer des bytes provenant du fob.
Si on ne peut recevoir toutes les donnees on renvoi une erreur.
entree:	data:adresse de sauvegarde des  donnees.
	nb_bytes:nombre de bytes a lire.
retour:1 si reussite 0 sinon.
*/
int get(unsigned char *data,int nb_bytes);

int getNumberData();

protected:
//variables
RS232_Mode mode_flux;//mode du controle de flux
int serial_handle;//handle d'ouverture
struct termios com_termio;//structure d'initialisation du port
struct termios old_termio;//sauvegarde de l'ancienne structure
char *com_port;//nom port RS-232
long baudrate;//vitesse du port
//methodes


};


///////////////////////////////////////////////////////////////////////////////

#endif // !defined RS232Interface_h

#undef RS232Interface_CYCLE
#endif // else defined RS232Interface_CYCLE

