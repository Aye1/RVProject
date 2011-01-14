///////////////////////////////////////////////////////////////////////////////
// File name : FobCommand.h
//
// Creation : 1999/03/16
// Version : 1.0
//
// Author : Raphael Grasset
// email : Raphael.Grasset@imag.fr
//
// Purpose : 
// interface de la classe fob : classe de gestion du fob.Cette classe
// est de bas niveau.elle implemente toutes les commandes possibles du fob.(certaines ont
// peut d'utilite.Cette classe ne gere pas les changements de repere et les modifications des donnees
// de facon geometrique : elle repond juste a l'implementation bas niveau de la gestion du fob.

// rem : commentaire a refaire
// actuellement le fob est fait pour marcher en standalone
// optimisation effectuer :lire la derniere valeur :fait plus haut

// Distribution :
//
// Use :
//	
//
// Todo :
//	O	Commentaire a refaire
//  O	constante a modifier
//  O	modifier la classe 
//		ou on rajouter pour permettre la connection de plusieurs fob.
//  0   ajouter timestamp
//	/
//	X
//
// History :
//	1999/03/16 : Mr Grasset : Create of the file
///////////////////////////////////////////////////////////////////////////////

#if defined FobCommand_CYCLE
#error Header cyclic inclusion detected in FobCommand.h
#else // defined FobCommand_CYCLE
#define FobCommand_CYCLE

#if !defined FobCommand_h
#define FobCommand_h

///////////////////////////////////////////////////////////////////////////////

#include "RS232Interface.h"
#include "math.h"
#include <sys/types.h>
#include <sys/prctl.h>
#include <signal.h>
#include <math.h>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// class FobCommand
///////////////////////////////////////////////////////////////////////////////


#define DTR (float)(3.141593/180.0)                 	/* degrees to radians */
#define RTD (float)57.2958				/* radians to degree */
#define FTD (float)180.0	                 	/* float to degree */
#define DTF (float)(1.0/180.0)				/* degree to float */
#define FTW (float)32768.0                          	/* float to word integer */
#define WTF (float)(1.0/32768.0)                    	/* word to  float integer */

#define POSK36 (float)36.0               		/* constante pour l'echelle */
#define POSK72 (float)72.0               		/* constante pour l'echelle */
#define POSK144 (float)144.0             		/* constante pour l'echelle */
#define B_VSYNCHRO 128


#define LEFT_PRESSED 16
#define MIDDLE_PRESSED 48
#define RIGHT_PRESSED 112

#define DEFAULTCRYSTALFREQ (float)(0.0)
#define DEFAULTCLOCKVALUE  (float)(0.0)

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef TRUE
    #define TRUE 1
#endif


/* Definition de types */

//type de mesures que peut effectues le FOB
enum Fob_RData {POSITION,ANGLES,MATRIX,QUATERNION,
POSITION_ANGLES,POSITION_MATRIX,POSITION_QUATERNION};

//type d'hemisphere sur lequel on peut travailler
enum Fob_HData {FORWARD,AFT,UPPER,LOWER,LEFT,RIGHT};

//type de vitesse de mesure (cycles de mesure)
enum Fob_Rated {D_1=81,D_2=82,D_8=83,D_32=84};

//type de synchro
enum Fob_Sync {NO_SYNC=0,CRT_SYNC1=1,CRT_SYNC2=2,HOST_SYNC=8};

//type de parametre pour les change/examine value
enum Fob_Param {BV_STATUS=0,BV_REVNUMBER=1,BV_CRYSTALF=2,BV_POSCALE=3,BV_FILTER=4,
BV_FILTERT=5,BV_RATEC=6,BV_RATE=7,BV_DATARM=8,BV_DATARV=9,
BV_ERROR=10,BV_ERRORM=11,BV_FTABLEVM=12,BV_FTABLEAM=13,BV_SCHANGEM=14,
BV_MODEL=15,BV_ERRORE=16,BV_REFRAME=17,BV_RESDELAY=32,BV_CONFIG=33,BV_ARMED=34,
BV_GROUPM=35,BV_FCONFIG=36,BV_AUTOC=50};

//type de filtre
enum FobFilter {FILTER_ON=0,FILTER_OFF=1};

//type d'echelle de valeurs
enum FobPosScale {POS_36,POS_72};

//type de masque d'erreur
enum FobErrorMask {BLINK_STOP=0,BLINK_RESUMES=1,RESUMES=3};

//type pour l'etat du FOB
enum FobActive {OFF,ON};


class FobCommand{

public:

//constructeur
FobCommand(const char *device_file="/dev/ttyS1",RS232_Mode mode=CONTROL_HARD,int frequency=B115200);

//destructeur
~FobCommand(); 

//methodes

//initialise le fob
void init();

//nettoie le port
void clear();
	
/*-----------------------gestion de l'emission-----------------------------------------*/

/* Run
(re)demarre le FOB.
*/
void Run();

/*Sleep()
arrete le FOB.
*/
void Sleep();

/* Xon()
permete l'envoi d'une commande XON.utiliser pour un flot de controle SOFT.voir doc termio.
*/
void Xon();

/* Xoff()
permete l'envoi d'une commande XON.utiliser pour un flot de controle SOFT.voir doc termio.
*/
void Xoff();

/*ToFbb
pas implemente
*/
void ToFbb(int number);

/*NextTransmitter
pas implemente
*/
void NextTransmitter();



/*-----------------------configuration donnees -----------------------------------------*/

/*SetAngleAlign
permet de definir le decalage entre le repere du FOB et le repere du transmetteur.
entrees : azimuth,elevation,roll : defini les 3 angles d'euler qui definissent l'orientation du repere du receveur
par rapport a celui du FOB.
*/
void SetAngleAlign(float azimuth=0.0, float elevation=0.0, float roll=0.0);

/*SetAngleAlign2
idem.la commande FOB est juste differente.
*/
void SetAngleAlign2(float azimuth=0.0, float elevation=0.0, float roll=0.0);
                                                                                                   
                                                                                                   
                         
/* SetReferenceFrame
permet de changer de referentiel de mesure.On peut changer l'orientation du repeteur du
transmetteur.On definit l'orientation du nouveau repere par rapport a l'ancien.
Si on veut que le changement influe aussi sur la position il faut d'abord envoyer au FOB une
commande CHANGE VALUE /XYZ REFERENCE FRAME / et 1 en parametre.
entrees:les 3 angles d'euler de changement de referentiel
*/
void SetReferenceFrame(float a=0.0, float e=0.0, float r=0.0);
/* SetReferenceFrame
idem.
*/
void SetReferenceFrame2(float a=0.0, float e=0.0, float r=0.0);

/* SetHemisphere
specifie dans quel hemisphere de l'espace de travail autour du FOB ou l'on souhaite 
travaille.Dans cette hemisphere, un axe est toujours de meme signe et les 2 autres varie.On definit
en quelques sorte un demi repere.
entree: hem:hemisphere choisi. 
*/
void SetHemisphere(Fob_HData hem);

/*SetRData
fixe le type de donnees que le fob doit renvoye: position,angles,matrice,quaternions...
entree:  type: le type de donnee a renvoye.
*/
void SetRData(Fob_RData type=POSITION_ANGLES);

/*

*/

void SetButtonMode(FobActive mode);

/*-----------------------gestion des donnees-----------------------------------------*/

/*Point
place le FOB en mode POINT.Apres l'envoi de cette commande le FOB renvoi un
enregistrement de donnee du mode choisi.L'utilisateur doit donc effectue une lecture
par un GetX ou X represente le mode choisi.
*/
void Point();

/*Stream
place le FOB en mode STREAM. apres l'envoi de cette commande le FOB envoi en mode continu des
donnees.Pour l'arreter il faut envoyer une commande POINT.Si on ne lit pas suffisament vite les
donnees dans le buffer hard de l'ordinateur on risque saturation et blocage de la machine
*/
void Stream();


/*Synchro
permet la resynchronisation du FOB. Utilise en mode STREAM,pour resynchronise le FOB en cas de
perte de donnee, mauvaise lecture...etc.... La routine consiste a lire octet par octet jusqu'a
retombe sur l'octet de synchronisation.(bit 8 a 1).cet octet est alors renvoye pour etre recupere.

retour:octet de synchro(qui est aussi une donnee) est debut d'un enregistrement.
*/
char Synchro();


/*GetX
Permet la lecture et la recuperation d'une donnee provenant du FOB.
les fonctions renvoi des tableaux de flottants.

GetPos: pos[0]=X pos[1]=Y pos[2]=Z renvoye en inch.
GetAngle: angle[0]=Zang=Azimuth angle[1]=Yang=Elevation angle[2]=Xang=Roll
GetQuaternions: quat[0]=q0 quat[1]=q1 quat[2]=q2 quat[3]=q3. qo est la composante scalaire.
GetMatrix : 
matrix[0]=m[1][1]=cos(y)*cos(z)
matrix[1]=m[2][1]=-cos(x)*sin(z)+sin(x)*sin(y)*cos(z)
matrix[2]=m[3][1]=sin(x)*sin(z)+cos(x)*sin(y)*cos(z)
matrix[3]=m[1][2]=cos(y)*sin(z)
matrix[4]=m[2][2]=cos(x)*cos(z)+sin(x)*sin(y)*sin(z)
matrix[5]=m[3][2]=-sin(x)*cos(z)+cos(x)*sin(y)*sin(z)
matrix[6]=m[1][3]=-sin(y)
matrix[7]=m[2][3]=sin(x)*cos(y)
matrix[8]=m[3][3]=cos(x)*cos(y)
*/

void GetPos(float pos[3]);
void GetAngle(float angle[3]);
void GetQuaternion(float quat[4]);
void GetMatrix(float matrix[9]);
void GetPosAngle(float pos[3],float angle[3]);
void GetPosMatrix(float pos[3],float matrix[9]);
void GetPosQuaternion(float pos[3],float quat[4]);
void GetPosQuaternionF(float pos[6],float quat[8]);//for two flock of birds
void GetPosQuaternionBF(float pos[6],float quat[8],int* button);//for two flock of birds

void GetButton(unsigned char *etat);
bool IsLeftPressed(unsigned char etat);
bool IsRightPressed(unsigned char etat);
bool IsMiddlePressed(unsigned char etat);
void GetPosB(float pos[3],unsigned char *etat);
void GetAngleB(float angle[3],unsigned char *etat);
void GetQuaternionB(float quat[4],unsigned char *etat);
void GetMatrixB(float matrix[9],unsigned char *etat);
void GetPosAngleB(float pos[3],float angle[3],unsigned char *etat);
void GetPosMatrixB(float pos[3],float matrix[9],unsigned char *etat);
void GetPosQuaternionB(float pos[3],float quat[4],unsigned char *etat);


/*-----------------------configuration materiel-----------------------------------------*/

/*SetRate
fixe la vitesse d'envoi des mesures par le FOB.On definit par rapport au cycle du FOB,la vitesse
de transmission du FOB : a tous les cycles, 1 cycle sur 2,1 cycle sur 8,1 cycle sur 32
entree:rated:vitesse choisi
*/
void SetRate(Fob_Rated rated);

/*SetSync
fixe le type de synchronisation desireee sur le FOB.Permet de se synchroniser avec un ecran
cathodique : on elimine le bruit genere par le fob sur l'ecran.
entree:sync:type de synchro choisi
*/
void SetSync(Fob_Sync sync);


void SetPosScal(FobPosScale f);
void SetFilter(FobFilter ACNarrowFilter,FobFilter ACWideFilter,FobFilter DCFilter);
void SetFilterTableAMin(float Talpha_min[7]);//0.008 a 0.9996
void SetMeasureRateCount(unsigned short rate);
void SetMeasureRate(unsigned short rate);
void SetDataReadyMode(FobActive readymode);
void SetDataReadyValue(unsigned char readychar);
void SetErrorMask(FobErrorMask mask);
void SetFilterTableAMax(float Talpha_max[7]);//0.008 a 0.9996
void SetFilterTableVm(unsigned short TVm[7]); // 0 a 32767
void SetSuddenChangeMode(FobActive smode);
void SetPositionReferenceFrame(FobActive rmode);
void SetResponseDelay(char delay);
void SetConfiguration();//pas present dans la doc
void SetFBBArmed(FobActive a);
void SetGroupMode(FobActive f);
void SetAutoConfiguration(int nb_flock);



void GetConfig();

void GetStatus();
void GetRevNumber();
void GetCrystalFreq();
void GetPosScal();
void GetFilter();
void GetFilterTableAMin();
void GetMeasureRateCount();
void GetMeasureRate();
void GetDataReadyMode();
void GetDataReadyValue();
void GetError();
void GetErrorMask();
void GetFilterTableVm();
void GetFilterTableAMax();
void GetSuddenChangeMode();
void GetModel();
void GetExpanderError();
void GetPositionReferenceFrame();
void GetResponseDelay();
void GetGroupMode();
void GetFlockConfig();
void GetConfiguration();//pas present dans la doc
void GetAutoConfiguration();
void GetSync();


protected:

//methodes

void Put(unsigned char *data,int nb_bytes);

void PutC(unsigned char data);

/*GetData
lit le nombre de caracteres places en parametre sur le FOB
entrees:nb_bytes: nombre de caractere a lire
entrees-sortie: l'adresse pour stocker les donnees resultat	
*/
void GetData(unsigned char *res,int nb_bytes);//lecture brute


/*DataToWord
converti les donnees provenant du Fob en Word.
entree:res:adresse de l'enregistrement a convertir
retour: le word resultat de la conversion
*/
short  DataToWord(unsigned char *res);

/*DataToFloat
converti les donnees provenant du Fob en Float.
entree:res:adresse de l'enregistrement a convertir
retour: le float resultat de la conversion
*/
float DataToFloat(unsigned char *res);

/*DataToX
converti les donnees BRUT recu du FOB en donnees exploitable.
*/
void DataToAngle(unsigned char res[6],float angle[3]);
void DataToPos(unsigned char res[6],float pos[3]);
void DataToQuaternion(unsigned char res[8],float quat[4]);
void DataToMatrix(unsigned char res[18],float matrix[9]);
void DataToPosAngle(unsigned char res[12],float pos[3],float angle[3]);
void DataToPosMatrix(unsigned char res[24],float pos[3],float matrix[9]);
void DataToPosQuaternion(unsigned char res[14],float pos[3],float quat[4]);


//variables

int scale_pos; //echelle des mesures de position 2 valeurs possibles:36 ou 72 
RS232Interface *intB;//port interface avec bird
int etat_trans;//etat du transmitter
char ** dcfiltermsg;
float crystalfreq;
float clockvalue;
int nb_flock; /* version 1 */
int GroupeMode;
static const char * bird_dcfiltermsg[];
/* pour le mode etendu :plusieurs controlleurs 
rajout attribut de type */
static const char * erc_dcfiltermsg[];


//commandes
static const unsigned char 	B_REPORT_RATE_1;
static const unsigned char 	B_REPORT_RATE_2;
static const unsigned char 	B_REPORT_RATE_8;
static const unsigned char 	B_REPORT_RATE_32;
static const unsigned char 	B_CHANGE_VALUE;
static const unsigned char 	B_EXAMINE_VALUE;
static const unsigned char 	B_FACTORY_TEST;
static const unsigned char 	B_SYNC;
static const unsigned char 	B_ANGLE_ALIGN;
static const unsigned char 	B_ANGLE_ALIGN2;
static const unsigned char 	B_REF_FRAME;
static const unsigned char 	B_REF_FRAME2;
static const unsigned char 	B_HEMISPHERE;
static const unsigned char 	B_RS232_TO_FBB;
static const unsigned char 	B_RS232_TO_FBB2;
static const unsigned char 	B_NEXT_TRANS;
static const unsigned char 	B_RUN;
static const unsigned char 	B_SLEEP;
static const unsigned char 	B_POINT;
static const unsigned char 	B_STREAM;
static const unsigned char 	B_XON;
static const unsigned char 	B_XOFF;
static const unsigned char 	B_ANGLES;
static const unsigned char 	B_POSITION;
static const unsigned char 	B_QUATERNION;
static const unsigned char 	B_MATRIX;
static const unsigned char 	B_POSITION_ANGLES;
static const unsigned char 	B_POSITION_MATRIX;
static const unsigned char 	B_POSITION_QUATERNION;
static const unsigned char 	B_BUTTON_MODE;
static const unsigned char 	B_BUTTON_READ;
//constantes de donnees hemisphere
static const unsigned char  *B_FORWARD;
static const unsigned char  *B_AFT;
static const unsigned char 	*B_UPPER;
static const unsigned char 	*B_LOWER ;
static const unsigned char 	*B_LEFT ;
static const unsigned char 	*B_RIGHT  ;
};


///////////////////////////////////////////////////////////////////////////////

#endif // !defined FobCommand_h

#undef FobCommand_CYCLE
#endif // else defined FobCommand_CYCLE
