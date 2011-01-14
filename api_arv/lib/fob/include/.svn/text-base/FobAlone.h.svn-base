///////////////////////////////////////////////////////////////////////////////
// File name : FobAlone.h
//
// Creation : 10/06/99
// Version : 1.0
//
// Author : Raphael Grasset
// email : Raphael.Grasset@imag.fr
//
// Purpose : classe interface d'utilisation du Fob. 
// Elle permet de recuperer directement les types de donnees voulu 
// dans le modele geometrique standard. Cette classe masque la gestion 
// et les commandes envoyes au Fob. elle permet de gerer un ou plusieurs 
// FOB utilise en mode 1 Fob / 1 RS232. le driver ne supporte pas 
// le mode etendu.(plus de 14 FOB).
//rem:
//le driver marche pour recuperer uniquement des positions et quaternions.
//implemente pour tous les types de donnees ? (utile ?)
//passer toutes les commandes en mode stream avec mutex sur donnees
//partagees:getPos getAngle ..etc..
//

// Distribution :
//
// Use :
//	
//
// Todo :
//	O	faire test si erreur de mode .(GetMatrix alors que mode POSITION
//	/
//	X
//
// History :
//	1999/06/10 : Mr Grasset : Create of the file
//	1999/11/06 : Mr Grasset : changement de la fonction d'initialisation
//	2000/06/07 : Mr Grasset : changement en sproc a la place de thread:

///////////////////////////////////////////////////////////////////////////////

#if defined FobAlone_CYCLE
#error Header cyclic inclusion detected in FobAlone.h
#else // defined FobAlone_CYCLE
#define FobAlone_CYCLE

#if !defined FobAlone_h
#define FobAlone_h

///////////////////////////////////////////////////////////////////////////////

#include <signal.h>
#include <unistd.h> 
#include <pthread.h> 
#include <semaphore.h>
//#include <ulocks.h>
#include <sys/time.h>
#include "FobCommand.h"

///////////////////////////////////////////////////////////////////////////////
// class FobAlone
///////////////////////////////////////////////////////////////////////////////


#define ITCM	(float)2.54


class FobAlone : public FobCommand {

public:

//constructeur
/*
type: defini le type de donnee que l'on veut utiliser:
choix possibles:POSITION, ANGLES, MATRIX, QUATERNION,
POSITION_ANGLES, POSITION_MATRIX, POSITION_QUATERNION.
hemisphere: defini l'hemisphere dans laquel on travaille
device_file: defini le port sur lequel est connecte le FOB
choix possible : FORWARD, AFT, UPPER, LOWER, LEFT (sur l'axe des x), RIGHT.
*/

FobAlone(Fob_RData type=POSITION_ANGLES,Fob_HData hemisphere=FORWARD,const char
*device_file="/dev/ttyS1",RS232_Mode mode_flux=CONTROL_HARD,int frequency=B115200);

//destructeur
~FobAlone() {SetRData(POSITION_ANGLES);}

//methodes

/*Config
permet de configurer les parametres utilisateurs du Fob
PAS IMPLEMENTE
*/
void Config();


/*InitAlone
Initialise le flock en mode standalone : 1 flock sur un port RS232.
rem:si on a plusieurs flocks sur plusieurs port RS232 initialise avec les
commandes InitSlave() et InitMaster()
*/
void InitAlone();

/*InitSlave
Initialisation du ou des fob slave en mode FLOCK avec une connection RS232 par
FLOCK
*/
void InitSlave();

/*
  Initialisation du ou des fob slave en mode FLOCK avec une connexion
  RS232 seulement sur le master FLOCK (appeler InitMaster sur le
  maÃ®tre)
*/
void InitSlaveUnSeulRS232(int numero_slave);


/*InitMaster
Initialisation du Fob maitre en mode FLOCK avec une connection RS232 par
FLOCK
entree:nb_flock defini le nombre de FOB dans le FLOCK.
*/
void InitMaster(int nb_flock);

/*SetScale
fixe l'echelle des valeurs de position retournes par le FOB.
le FOB renvoi par defaut  des valeurs entre -36*2.54 et 36*2.54 cm.
entrees:x,y,z valeurs scalaire sur les 3 axes
*/
void SetScale(float x,float y,float z);

/*SetPosition
fixe la position physique courante du Fob comme position defini en parametre.
entrees:
*/
void SetPosition(float x,float y,float z);

/*IncrPosition
permet d'incrementer la position du repere du transmetteur.
entrees:x,y,z les 3 composantes sur les 3 axes.
*/
void IncrPosition(float x,float y,float z);

/* SetOrientation
fixe l'orientation physique courante du Fob comme orientation defini en parametre.
*/
void SetOrientation(float azimuth,float elevation,float roll);

/* SetOrientationCenter 
permet de fixer l'orientation actuelle physique du FOB comme nouveau repere
*/
void SetOrientationCenter();

/* SetPositionCenter 
permet de fixer la position actuelle physique du FOB comme nouveau repere du transmetteur.
*/
void SetPositionCenter();

/* SetCenter
permet de fixer la position et l'orientation actuelle du FOB
comme nouveau repere.
void SetCenter();
*/

/*Reset
reinitialise l'alignement et la position du FOB.
*/
void Reset();

/*------------------------------fonctionnement en mode point------------------------------*/
/*rem : Attention on doit etre dans le bon mode pour appelle la commande correspondante.Aucune
verification n'est realise si on a pas la bonne correspondance.
Les commandes suivantes utilise le mode POINT:une commande POINT est envoye au FOB a chaque
appel,et une lecture des donnees est effectues.Tandis qu'en mode STREAM les donnees sont
envoyes en contigu sans rappel de commande.
*/

/*GetPos
permet de recuperer la position courante du FOB.
sorties:pos[0]=x pos[1]=y pos[2]=z en cm suivant l'echelle choisi.

rem:etre en mode POSITION.
*/
void GetPos(float pos[3]);

/*GetAngle
permet de recuperer l'orientation courante du FOB sous forme d'angle.
sorties:angle[0]=Xang=Roll angle[1]=Yang=Elevation angle[2]=Azimuth.

rem:le fob est limite dans ce mode: Yang varie juste entre -90/+90 degree.pour une
orientation plus stable utilise le mode matrice ou quaternion.
rem:etre en mode ANGLES.
*/
void GetAngle(float angle[3]);

/*GetMatrix
permet de recuperer l'orientation courante du FOB sous forme de matrice.
sortie:matrix:une matrice homogene de rotation.
rem:etre en mode MATRIX.
*/
void GetMatrix(float matrix[16]);

/*GetQuaternion
permet de recuperer l'orientation courante du FOB sous forme de quaternion.
sortie:quat[0]=q0 quat[1]=q1 quat[2]=q2 quat[3]=q3 avec q3 composante scalaire.

rem:etre en mode QUATERNION.
*/
void GetQuaternion(float quat[4]);


/* les modes suivants sont les modes combines des precedents */
void GetPosAngle(float pos[3],float angle[3]);//mode POSITION_ANGLES
void GetPosMatrix(float pos[3],float matrix[16]);//mode POSITION_MATRIX
void GetPosQuaternion(float pos[3],float quat[4]);//mode POSITION_QUATERNION

/*GetMatrixGL
renvoi une matrice homogene comprenant rotation+translation.
sortie:matrix la matrice homogene.
rem:etre en mode POSITION_MATRIX
*/
void GetMatrixGL(float matrix[16]);


/*------------------------------fonctionnement en mode stream------------------------------*/

/*RunDriver
lance un thread d'ecoute du fob utilise en mode stream.Le driver ne renvoi que des positions et
quaternions.il n'implemente pas les autres mode.on utilise la norme POSIX.
*/
void RunDriver();

/*StopDriver
arrete le Driver.(stop le mode stream et tue le thread.
*/
void StopDriver();

/*GetSPosQuaternion
recupere la position et l'orientation actuelle du Fob.
pos[0]=x pos[1]=y pos[2]=z.
quat[0]=q0 quat[1]=q1 quat[2]=q2 quat[3]=q3
*/
void GetSPosQuaternion(float pos[3],float quat[4]);

//attributs

float shared_pos[3],shared_quat[4];//donnees partagees
int axis_1,axis_2;//axe de changement d'hemisphere 0=x 1=y 2=z
 pthread_mutex_t mutex;//mutex sur les donnees

protected:

//methodes

/*RotateHem
transforme l'hemisphere transmis en hemisphere pour le FOB.cette fonction est utilise car le
FOB ne tient pas compte de notre changement de repere pour cette commande.
*/
Fob_HData RotateHem(Fob_HData h);

/* Init
Initialise le FOB:reglage interne + connection sur le port serie
*/
void Init();

//attributs
Fob_RData mode;//mode d'utilisation du FOB
Fob_HData hem;//hemisphere de travail
float scale_x,scale_y,scale_z;//echelle entre le repere du transmetteur et le repere geometrique
float pos_x,pos_y,pos_z;//position du repere transmetteur.

 
 pthread_t fob_thread;//thread du driver
 
};

///////////////////////////////////////////////////////////////////////////////

#endif // !defined FobAlone_h

#undef FobAlone_CYCLE
#endif // else defined FobAlone_CYCLE

