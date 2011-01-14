#include <iostream>
#include "FobCommand.h"

const unsigned char 	FobCommand::B_REPORT_RATE_1=81;
const unsigned char 	FobCommand::B_REPORT_RATE_2=82;
const unsigned char 	FobCommand::B_REPORT_RATE_8=83;
const unsigned char 	FobCommand::B_REPORT_RATE_32=84;
const unsigned char 	FobCommand::B_CHANGE_VALUE=80;
const unsigned char 	FobCommand::B_EXAMINE_VALUE=79;
const unsigned char 	FobCommand::B_FACTORY_TEST=122;
const unsigned char 	FobCommand::B_SYNC=65;
const unsigned char 	FobCommand::B_ANGLE_ALIGN=74;
const unsigned char 	FobCommand::B_ANGLE_ALIGN2=113;
const unsigned char 	FobCommand::B_REF_FRAME=72;
const unsigned char 	FobCommand::B_REF_FRAME2=114;
const unsigned char 	FobCommand::B_HEMISPHERE=76;
const unsigned char 	FobCommand::B_RS232_TO_FBB=240;
const unsigned char 	FobCommand::B_RS232_TO_FBB2=224;
const unsigned char 	FobCommand::B_NEXT_TRANS=48;
const unsigned char 	FobCommand::B_RUN=70;
const unsigned char 	FobCommand::B_SLEEP=71;
const unsigned char 	FobCommand::B_POINT=66;
const unsigned char 	FobCommand::B_STREAM=64;
const unsigned char 	FobCommand::B_XON=17;
const unsigned char 	FobCommand::B_XOFF=19;
const unsigned char 	FobCommand::B_ANGLES=87;
const unsigned char 	FobCommand::B_POSITION=86;
const unsigned char 	FobCommand::B_QUATERNION=92;
const unsigned char 	FobCommand::B_MATRIX=88;
const unsigned char 	FobCommand::B_POSITION_ANGLES=89; 
const unsigned char 	FobCommand::B_POSITION_MATRIX=90 ;
const unsigned char 	FobCommand::B_POSITION_QUATERNION=93;
const unsigned char 	FobCommand::B_BUTTON_MODE=77 ;
const unsigned char 	FobCommand::B_BUTTON_READ=78;

const unsigned char  *FobCommand::B_FORWARD=(const unsigned char*)"\x00\x00";
const unsigned char  *FobCommand::B_AFT=(const unsigned char*)"\x00\x01";
const unsigned char 	*FobCommand::B_UPPER=(const unsigned char*)"\x0C\x01";
const unsigned char 	*FobCommand::B_LOWER=(const unsigned char*)"\x0C\x00";
const unsigned char 	*FobCommand::B_LEFT=(const unsigned char*)"\x06\x01";
const unsigned char 	*FobCommand::B_RIGHT=(const unsigned char*)"\x06\x00";

const char * FobCommand::bird_dcfiltermsg[] = {"0 to 12 inches",
				   "12 to 15 inches",
				   "15 to 19 inches",
				   "19 to 24 inches",
				   "24 to 30 inches",
				   "30 to 38 inches",
				   "38+ inches"};
/* pour le mode etendu :plusieurs controlleurs 
rajout attribut de type */
const char * FobCommand::erc_dcfiltermsg[] = {"0 to 35 inches",
				   "35 to 49 inches",
				   "49 to 63 inches",
				   "63 to 79 inches",
				   "79 to 96 inches",
				   "96 to 116 inches",
				   "116+ inches"};
	

			   
FobCommand::FobCommand(const char *device_file,RS232_Mode mode,int frequency)
{	
  // intB=new RS232Interface(device_file,B115200,mode);
  // intB=new RS232Interface(device_file,B38400,mode);
  intB=new RS232Interface(device_file,frequency,mode);
  nb_flock=1;//on a un flock
  GroupeMode=FALSE;//on est pas en groupe mode
  crystalfreq=DEFAULTCRYSTALFREQ;
  scale_pos=POSK36;//echelle 36 pouces
}

FobCommand::~FobCommand()
{

	std::cout<<"deconnexion .." << std::endl;
	intB->disconnect();
	delete intB;
}
//mode standalone
void
FobCommand::Put(unsigned char *data,int nb_bytes)
{
intB->put(data,nb_bytes);
}

void
FobCommand::PutC(unsigned char data)
{
intB->put_c(data);
}

/*mode flock
void
FobCommand::Put(unsigned char *data,int nb_bytes)
{
unsigned char *data2=new unsigned char[nb_bytes+1];
data2[0]=GetAddress();
for (i=0;i<nb_bytes;i++)
	data2[i+1]=data[i];
intB->(data2,nb_bytes+1);
}

void
FobCommand::PutC(unsigned char data)
{
unsigned char data2[2];
data2[0]=GetAddress();
data2[1]=data;
intB->(data2,2);
}
*/

void
FobCommand::init()
{
	intB->connect();
}

void
FobCommand::clear()
{
    intB->clearBuffer();
}
/*-----------------------gestion de l'emission-----------------------------------------*/

void
FobCommand::Run()
{

	etat_trans=1;
	PutC(B_RUN);
}

void
FobCommand::Sleep()
{

	etat_trans=0;
	PutC(B_SLEEP);
}

void
FobCommand::Xon()
{
	PutC(B_XON);
}

void
FobCommand::Xoff()
{
	PutC(B_XOFF);
}


void
FobCommand::ToFbb(int number)
{
    PutC(B_RS232_TO_FBB+number);
}

void
FobCommand::NextTransmitter()
{
}

		
/*-----------------------configuration donnees -----------------------------------------*/
		

void 
FobCommand::SetAngleAlign(float azimuth,float elevation,float roll)
{
	unsigned char buffer[13];
	float tempfloat;        
	float angle[3];
	int i,j=1;
	
	// on utilise notre structure 
	angle[0]=azimuth;
	angle[1]=elevation;
	angle[2]=roll;

	buffer[0]=B_ANGLE_ALIGN;

	for (i=0;i<3;i++)
	{// on calcule le sinus de l'angle 
		tempfloat = sin((double)(angle[i] * DTR));
	/*NOTE: trap for sin(90)...since the bird
        can only accept data from -1.000 to 0.99996 (0x8000 to 0x7fff)*/
		if (tempfloat < 0.99997)
		{
	    		buffer[j++] = (unsigned char) ((short) (tempfloat * FTW) & 0x0ff);
	    		buffer[j++] = (unsigned char) (((short) (tempfloat * FTW) & 0x0ff00) >> 8);
		}
		else
		{
		    	buffer[j++]= 0x0ff;
	    		buffer[j++]= 0x07f;
		}

		tempfloat = cos((double)(angle[i] * DTR));
		if (tempfloat < 0.99997)
		{
	    		buffer[j++] = (unsigned char) ((short) (tempfloat * FTW) & 0x0ff);
	    		buffer[j++]= (unsigned char) (((short) (tempfloat * FTW) & 0x0ff00) >> 8);
		}
		else
		{
	    		buffer[j++] = 0x0ff;
	    		buffer[j++] = 0x07f;
		}
    	}
	// envoi commande + data
	Put(buffer,13);
}


void
FobCommand::SetAngleAlign2(float azimuth,float elevation,float roll)
{
	unsigned char buffer[7];   
	float angle[3];
	int i,j=1;
	// on utilise notre structure et on converti en flottant entre 0 et 1
	angle[0]=azimuth*DTF;
	angle[1]=elevation*DTF;
	angle[2]=roll*DTF;

	buffer[0]=B_ANGLE_ALIGN2;

	for (i=0;i<3;i++)
	{
		buffer[j++] = (unsigned char) ((unsigned short) (angle[i] * FTW) & 0x0ff);
		buffer[j++] = (unsigned char) (((unsigned short) (angle[i] * FTW) & 0x0ff00) >> 8);
	}	
	// envoi commande + data 
	Put(buffer,7);

}


void 
FobCommand::SetReferenceFrame(float azimuth,float elevation,float roll)
{
	unsigned char buffer[13];
	float tempfloat;        
	float angle[3];
	int i,j=1;

	// initialise structure intermediaire 
	angle[0]=azimuth;
	angle[1]=elevation;
	angle[2]=roll;

	buffer[0]=B_REF_FRAME;
	
	for (i=0;i<3;i++)
	{// on calcule le sinus de l'angle 
		tempfloat = sin((double)(angle[i] * DTR));
	/*NOTE: trap for sin(90)...since the bird
	can only accept data from -1.000 to 0.99996 (0x8000 to 0x7fff)*/

		if (tempfloat < 0.99997)
		{
	    		buffer[j++] = (unsigned char) ((short) (tempfloat * FTW) & 0x0ff);
	    		buffer[j++] = (unsigned char) (((short) (tempfloat * FTW) & 0x0ff00) >> 8);
		}
		else
		{
	    		buffer[j++]= 0x0ff;
	    		buffer[j++]= 0x07f;
		}

		tempfloat = cos((double)(angle[i] * DTR));
		
		if (tempfloat < 0.99997)
		{
	    		buffer[j++] = (unsigned char) ((short) (tempfloat * FTW) & 0x0ff);
	    		buffer[j++]= (unsigned char) (((short) (tempfloat * FTW) & 0x0ff00) >> 8);
		}
		else
		{
	    		buffer[j++] = 0x0ff;
	    		buffer[j++] = 0x07f;
		}
    	}

	// envoi commande + data 
	Put(buffer,13);
}

void
FobCommand::SetReferenceFrame2(float azimuth, float elevation, float roll)
{
	unsigned char buffer[7];       
	float angle[3];
	int i,j=1;

	// initialise structure intermediaire 
	angle[0]=azimuth*DTF;
	angle[1]=elevation*DTF;
	angle[2]=roll*DTF;

	buffer[0]=B_REF_FRAME;
	
	for (i=0;i<3;i++)
	{	    
		buffer[j++] = (unsigned char) ((short) (angle[i] * FTW) & 0x0ff);	    
		buffer[j++] = (unsigned char) (((short) (angle[i] * FTW) & 0x0ff00) >> 8);
	}
	// envoi commande + data 
	Put(buffer,7);
}


void
FobCommand::SetHemisphere(Fob_HData hem)
{
	unsigned char d_hemis[3];
	
	d_hemis[0]=B_HEMISPHERE;
	switch (hem)
	{
   		case FORWARD:d_hemis[1]=B_FORWARD[0];d_hemis[2]=B_FORWARD[1];break;
   		case AFT: d_hemis[1]=B_AFT[0];d_hemis[2]=B_AFT[1];break;
   		case UPPER:d_hemis[1]=B_UPPER[0];d_hemis[2]=B_UPPER[1];break;
   		case LOWER:d_hemis[1]=B_LOWER[0];d_hemis[2]=B_LOWER[1];break;
   		case LEFT: d_hemis[1]=B_LEFT[0];d_hemis[2]=B_LEFT[1];break;
   		case RIGHT:d_hemis[1]=B_RIGHT[0];d_hemis[2]=B_RIGHT[1];break;
   		default: std::cerr<<"ERREUR:valeur non valide !" << std::endl;exit(1);break;
   	}
	Put(d_hemis,3);
}

/*-----------------------gestion des donnees-----------------------------------------*/

void
FobCommand::Point()
{
	PutC(B_POINT);
}


void
FobCommand::SetRData(Fob_RData type)
{

	unsigned char choice = 0;

	switch  (type)
	{
		case POSITION:choice=B_POSITION;break;
		case ANGLES: choice=B_ANGLES;break;
		case MATRIX:choice=B_MATRIX;break;
		case QUATERNION: choice=B_QUATERNION;break;
		case POSITION_ANGLES:choice=B_POSITION_ANGLES;break;
		case POSITION_MATRIX :choice=B_POSITION_MATRIX;break;
		case POSITION_QUATERNION:choice=B_POSITION_QUATERNION;break;
   
	}
	PutC(choice);
}

void
FobCommand::SetButtonMode(FobActive mode)
{
unsigned char buffer[2];
buffer[0]=B_BUTTON_MODE;
buffer[1]=mode;
Put(buffer,2);
}

void
FobCommand::Stream()
{
	PutC(B_STREAM);
}

char
FobCommand::Synchro()
{

	unsigned char synchro;
	char est_synchro=0;

	do
	{
		intB->get(&synchro,1);
		if (synchro&B_VSYNCHRO)	
			est_synchro=1;
	}
	while (!est_synchro);
	return synchro;
}


void
FobCommand::GetData(unsigned char *res,int nb_bytes)
{
    intB->get(res,nb_bytes);
    if (!(res[0]&B_VSYNCHRO))//mauvaise synchro
    {
	//cerr<<"NOTIFY->FOBCOMMAND/GETDATA:mauvaise synchronisation"<<endl;
	res[0]=Synchro();
	intB->get(&(res[1]),nb_bytes-1);
    }
    res[0]=res[0]&127;/* on enleve le bit de synchro */
}

void 
FobCommand::GetPos(float pos[3])
{
	unsigned char res[6];
	
	GetData(res,6);
	DataToPos(res,pos);
}

void 
FobCommand::GetAngle(float angle[3])
{
	unsigned char res[6];
	
	GetData(res,6);
	DataToPos(res,angle);
}

void 
FobCommand::GetQuaternion(float quat[4])
{
	unsigned char res[8];
	
	GetData(res,8);
	DataToQuaternion(res,quat);
}

void 
FobCommand::GetMatrix(float matrix[9])
{
	unsigned char res[18];
	
	GetData(res,18);
	DataToMatrix(res,matrix);
}


void
FobCommand::GetPosAngle(float pos[3],float angle[3])
{
	unsigned char res[12];
	
	GetData(res,12);
	DataToPosAngle(res,pos,angle);
}

void 
FobCommand::GetPosMatrix(float pos[3],float matrix[16])
{
	unsigned char res[24];
	
	GetData(res,24);
	DataToPosMatrix(res,pos,matrix);
}

void 
FobCommand::GetPosQuaternion(float pos[3],float quat[4])
{
	unsigned char res[14];
	
	GetData(res,14);
	DataToPosQuaternion(res,pos,quat);
}

//use just for 2 fob !!
//very very bad code !! no time !!!
void 
FobCommand::GetPosQuaternionF(float pos[6],float quat[8])
{
	unsigned char res[30];
	float postemp[3];
	float quattemp[4];

	GetData(res,30);

	DataToPosQuaternion(res,postemp,quattemp);
	pos[0]=postemp[0];
	pos[1]=postemp[1];
	pos[2]=postemp[2];
	quat[0]=quattemp[0];
	quat[1]=quattemp[1];
	quat[2]=quattemp[2];
	quat[3]=quattemp[3];
	DataToPosQuaternion(res+15,postemp,quattemp);
	pos[3]=postemp[0];
	pos[4]=postemp[1];
	pos[5]=postemp[2];
	quat[4]=quattemp[0];
	quat[5]=quattemp[1];
	quat[6]=quattemp[2];
	quat[7]=quattemp[3];     
}

void 
FobCommand::GetPosQuaternionBF(float pos[6],float quat[8],int* button)
{
	unsigned char res[31];
	float postemp[3];
	float quattemp[4];

	GetData(res,31);

	DataToPosQuaternion(res,postemp,quattemp);
	pos[0]=postemp[0];
	pos[1]=postemp[1];
	pos[2]=postemp[2];
	quat[0]=quattemp[0];
	quat[1]=quattemp[1];
	quat[2]=quattemp[2];
	quat[3]=quattemp[3];   
	*button=(int)(res[14]);//suppose to be after the first 7 value
	DataToPosQuaternion(res+16,postemp,quattemp);
	pos[3]=postemp[0];
	pos[4]=postemp[1];
	pos[5]=postemp[2];
	quat[4]=quattemp[0];
	quat[5]=quattemp[1];
	quat[6]=quattemp[2];
	quat[7]=quattemp[3];     
}

void 
FobCommand::GetButton(unsigned char *etat)
{
	unsigned char commande[1];
	unsigned char buffer[1];
	
	commande[0]=B_BUTTON_READ;
	Put(commande,1);
	intB->get(buffer,1);
	*etat=buffer[0];
}
bool 
FobCommand::IsLeftPressed(unsigned char etat)
{
	return (etat==LEFT_PRESSED);
}

bool 
FobCommand::IsRightPressed(unsigned char etat)
{
	return (etat==RIGHT_PRESSED);
}

bool 
FobCommand::IsMiddlePressed(unsigned char etat)
{
	return (etat==MIDDLE_PRESSED);
}

void 
FobCommand::GetPosB(float pos[3],unsigned char *etat)
{
	unsigned char res[7];
	
	GetData(res,7);
	*etat=res[6];
	DataToPos(res,pos);
}

void 
FobCommand::GetAngleB(float angle[3],unsigned char *etat)
{
	unsigned char res[7];
	
	GetData(res,7);
	*etat=res[6];
	DataToPos(res,angle);
}

void 
FobCommand::GetQuaternionB(float quat[4],unsigned char *etat)
{
	unsigned char res[9];
	
	GetData(res,9);
	*etat=res[8];
	DataToQuaternion(res,quat);
}

void 
FobCommand::GetMatrixB(float matrix[9],unsigned char *etat)
{
	unsigned char res[19];
	
	GetData(res,19);
	*etat=res[18];
	DataToMatrix(res,matrix);
}


void
FobCommand::GetPosAngleB(float pos[3],float angle[3],unsigned char *etat)
{
	unsigned char res[13];
	
	GetData(res,13);
	*etat=res[12];
	DataToPosAngle(res,pos,angle);
}

void 
FobCommand::GetPosMatrixB(float pos[3],float matrix[16],unsigned char *etat)
{
	unsigned char res[25];
	
	GetData(res,25);
	*etat=res[24];
	DataToPosMatrix(res,pos,matrix);
}


void 
FobCommand::GetPosQuaternionB(float pos[3],float quat[4],unsigned char *etat)
{
	unsigned char res[15];
	
	GetData(res,15);
	*etat=res[14];
	DataToPosQuaternion(res,pos,quat);
}

void 
FobCommand::DataToAngle(unsigned char res[6],float angle[3])
{
	angle[0]=DataToFloat(&res[0])*FTD;
	angle[1]=DataToFloat(&res[2])*FTD;
	angle[2]=DataToFloat(&res[4])*FTD;
}

void 
FobCommand::DataToPos(unsigned char res[6],float pos[3])
{
	pos[0]=DataToFloat(&res[0])*scale_pos;
	pos[1]=DataToFloat(&res[2])*scale_pos;
	pos[2]=DataToFloat(&res[4])*scale_pos;
}

void 
FobCommand::DataToQuaternion(unsigned char res[8],float quat[4])
{
	quat[0]=DataToFloat(&res[0]);
	quat[1]=DataToFloat(&res[2]);
	quat[2]=DataToFloat(&res[4]);
	quat[3]=DataToFloat(&res[6]);
}

void 
FobCommand::DataToPosAngle(unsigned char res[12],float pos[3],float angle[3])
{
	pos[0]=DataToFloat(&res[0])*scale_pos;
	pos[1]=DataToFloat(&res[2])*scale_pos;
	pos[2]=DataToFloat(&res[4])*scale_pos;
	angle[0]=DataToFloat(&res[6])*FTD;
	angle[1]=DataToFloat(&res[8])*FTD;
	angle[2]=DataToFloat(&res[10])*FTD;
}


void 
FobCommand::DataToMatrix(unsigned char res[18],float matrix[9])
{
	matrix[0]=DataToFloat(&res[0]);
	matrix[1]=DataToFloat(&res[2]);
	matrix[2]=DataToFloat(&res[4]);
	matrix[3]=DataToFloat(&res[6]);
	matrix[4]=DataToFloat(&res[8]);
	matrix[5]=DataToFloat(&res[10]);
	matrix[6]=DataToFloat(&res[12]);
	matrix[7]=DataToFloat(&res[14]);
	matrix[8]=DataToFloat(&res[16]);
}

void 
FobCommand::DataToPosMatrix(unsigned char res[24],float pos[3],float matrix[9])
{
	pos[0]=DataToFloat(&res[0])*scale_pos;
	pos[1]=DataToFloat(&res[2])*scale_pos;
	pos[2]=DataToFloat(&res[4])*scale_pos;
	matrix[0]=DataToFloat(&res[6]);
	matrix[1]=DataToFloat(&res[8]);
	matrix[2]=DataToFloat(&res[10]);
	matrix[3]=DataToFloat(&res[12]);
	matrix[4]=DataToFloat(&res[14]);
	matrix[5]=DataToFloat(&res[16]);
	matrix[6]=DataToFloat(&res[18]);
	matrix[7]=DataToFloat(&res[20]);
	matrix[8]=DataToFloat(&res[22]);
}

void 
FobCommand::DataToPosQuaternion(unsigned char res[14],float pos[3],float quat[4])
{
	pos[0]=DataToFloat(&res[0])*scale_pos;
	pos[1]=DataToFloat(&res[2])*scale_pos;
	pos[2]=DataToFloat(&res[4])*scale_pos;
	quat[0]=DataToFloat(&res[6]);
	quat[1]=DataToFloat(&res[8]);
	quat[2]=DataToFloat(&res[10]);
	quat[3]=DataToFloat(&res[12]);
}

float 
FobCommand::DataToFloat(unsigned char *res)
{
	return 	(float)(((short) ((((short)res[0]&0x7F)|((short)res[1]<<7)) <<2))*WTF);
//return (((short)((((short)res[0]<<1)|((short)res[1]<<8))<<1))*WTF);
}

short 
FobCommand::DataToWord(unsigned char *res)
{
	return ((short) ((((short)res[0]&0x7F)|((short)res[1]<<7)) <<2));
//return ((short)((((short)res[0]<<1)|((short)res[1]<<8))<<1));
}



void
FobCommand::SetRate(Fob_Rated rated)
{

	unsigned char val;
	val=(unsigned char)rated;
	Put(&val,1);
}

void
FobCommand::SetSync(Fob_Sync sync)
{
	unsigned char commande[2];
	commande[0]=B_SYNC;
	commande[1]=sync;
	Put(commande,2);
}

/*
on ne se positionne pas en
attente */
void 
FobCommand::GetStatus()
{

	unsigned char commande[2];
	unsigned char buffer[2];

	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_STATUS;
	Put(commande,2);
	intB->get(buffer,2);
	
	if ( (buffer[1]) & 0x80 )
		printf("Bird is Master\n\r");
	else
		printf("Bird is Slave\n\r");
	/* check bird initialization status .. bit 14 */
	if ( (buffer[1]) & 0x40 )
		printf("Bird has been initialized\n\r");
	else
		printf("Bird has not been initialized\n\r");

	/*  check if an error has been detected .. bit 13 */
	if ( (buffer[1]) & 0x20 )
		printf("An error has been detected\n\r");
	else
		printf("No error detected\n\r");

	/* check if bird is NOT RUNNING / RUNNING .. bit 12 */
	if ( (buffer[1]) & 0x10 )
		printf("Bird is RUNNING\n\r");
	else
		printf("Bird is NOT RUNNING\n\r");
	/* check if bird is in HOST SYNC .. bit 11 */
	if ( (buffer[1]) & 0x08 )
		printf("Bird is in HOST SYNC\n\r");
	else
		 printf("Bird is not in HOST SYNC\n\r");

	/* Print Expanded Addressing or Expanded .. bit 10 */
	if ( (buffer[1]) & 0x04 )
		printf("Bird in Expanded Addressing Mode\n\r");
	else
		printf("Bird in Normal Addressing Mode\n\r");

	/* check if in CRTSYNC mode .. bit 9 */
	if ( (buffer[1]) & 0x02 )
		printf("Bird in CRTSYNC Mode\n\r");
	else
		printf("Bird not in CRTSYNC Mode\n\r");
	/* check if sync modes enabled .. bit 8 */
	if ( (buffer[1]) & 0x01 )
		printf("SYNC mode not enabled\n\r");
	else
		printf("SYNC mode enabled\n\r");

	/* check factory test mode ..bit 7 */
	printf ("Factory Test Commands: ");
	if ((buffer[0]) & 0x80)
		printf("ENABLED\n\r");
	else
		printf("DISABLED\n\r");

	/* check XOFF/XON ..bit 6  */
	if ((buffer[0]) & 0x80)
		printf("XOFF\n\r");
	else
		printf("XON\n\r");
	
	/* check sleep/wake ..bit 5 */
	if ((buffer[0]) & 0x20)
		printf("Bird in Sleep Mode\n\r");
	else
		printf("Bird in Wake Up Mode\n\r");

	/* check output selection ..bits 4,3,2,1 */
	switch ((buffer[0] & 0x1e) >> 1)
	{
		case 1:printf ("Position output selected ");break;
		case 2:printf ("Angle output selected ");break;
		case 3:printf ("Matrix output selected ");break;
		case 4:printf ("Position/Angle output selected ");break;
		case 5:printf ("Position/Matrix output selected ");break;
		case 6:printf ("Factory Test output selected ");break;
		case 7:printf ("Quaternion output selected ");break;
		case 8:printf ("Position/Quaternion output selected ");break;
		default:printf ("Illegal Output mode detected\n\r");
	}
	/* check outputmode .. bit 0 */
	if ((buffer[0]) & 0x1)
		printf("in STREAM mode\n\r");
	else
		printf("in POINT mode\n\r");
}



void 
FobCommand::GetRevNumber()
{
	unsigned char commande[2];
	unsigned char buffer[2];

	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_REVNUMBER;
	Put(commande,2);
	intB->get(buffer,2);
	
	printf ("Software REV %d.%d\n\r",buffer[0],buffer[1]);
}


void 
FobCommand::GetCrystalFreq()
{
	unsigned char commande[2];
	unsigned char buffer[2];
	
	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_CRYSTALF;	
	Put(commande,2);
	intB->get(buffer,2);
	
	printf ("CPU Crystal is %4.1i MHz\n\r",buffer[0]);
	crystalfreq=buffer[0];
}	
    

  
void 
FobCommand::GetPosScal()
{
	unsigned char commande[2];
	unsigned char buffer[2];

	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_POSCALE;
	Put(commande,2);
	intB->get(buffer,2);
	
	printf ("Maximum Range is ");
	if (buffer[0] == 1)
		printf ("72 inches\n\r");
	else
		printf ("36 inches\n\r");
}	    



void 
FobCommand::SetPosScal(FobPosScale f)
{
	unsigned char commande[3];

	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_POSCALE;
	commande[2]=f;
	
	if (f==POS_36)
		scale_pos=POSK36;
	else
		scale_pos=POSK72;
	Put(commande,3);	
}
     
void 
FobCommand::GetFilter()
{
	unsigned char commande[2];
	unsigned char buffer[2];

	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_FILTER;
	Put(commande,2);
	intB->get(buffer,2);

	printf ("AC Narrow Notch filter is: ");
	if (buffer[0] & 4)
		printf ("OFF\n\r");
	else
		printf ("ON\n\r");
	printf ("AC Wide Notch filter is: ");
	if (buffer[0] & 2)
		printf ("OFF\n\r");
	else
		printf ("ON\n\r");
	printf ("DC filter is: ");
	if (buffer[0] & 1)
		printf ("OFF\n\r");
	else
		printf ("ON\n\r");
}



void 
FobCommand::SetFilter(FobFilter ACNarrowFilter,FobFilter ACWideFilter,FobFilter DCFilter)
{
	unsigned char commande[4];
	
	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_FILTER;
	commande[2]=(ACNarrowFilter<<2)+(ACWideFilter<<1)+DCFilter;
	commande[3]=0;
	Put(commande,4);
}

void 
FobCommand::GetFilterTableAMin()
{
	unsigned char commande[2];
	unsigned char buffer[14];
	int i;

	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_FILTERT;
	Put(commande,2);
	intB->get(buffer,14);
	
	/* mode normal :modifie pour etendu 
   	dcfiltermsg = bird_dcfiltermsg;*/
	for (i=0; i<7; i++)
	{
	printf ("For the range %s, Filter constant is %6.4f\n\r",bird_dcfiltermsg[i],
			    (float)(((*(unsigned char *)&buffer[(i*2)]) +
				    ((*(unsigned char *)&buffer[(i*2+1)]) << 8))
				     * WTF));
		
	}
}

/* s'assurer que  0..008<Talpha_min[i]<0.99996<ALPHA_MAX */
void
FobCommand::SetFilterTableAMin(float Talpha_min[7])
{
	unsigned char commande[16];
	int i,j=2;
	
	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_FILTERT;
	
	for (i=0;i<7;i++)
	{	
		commande[j++] = (unsigned char) ((short) (Talpha_min[i] * FTW) & 0x0ff);
		commande[j++]= (unsigned char) (((short) (Talpha_min[i] * FTW) & 0x0ff00) >> 8);
	}
	Put(commande,16);
}

void 
FobCommand::GetMeasureRateCount()
{
	unsigned char commande[2];
	unsigned char buffer[2];
	unsigned short xmtr_time_count;
	float measurementrate;

	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_RATEC;
	Put(commande,2);
	intB->get(buffer,2);	

	xmtr_time_count = ((*(unsigned char *)&buffer[0]) & 0xff)
		    + (((*(unsigned char *)&buffer[1]) & 0xff) << 8) ;
	measurementrate =1000.0/((4*(xmtr_time_count*(8.0/crystalfreq)/1000.0))+ 0.3);
	printf ("Measurement Rate: %6.2f\n\r",measurementrate);
}

// les valeurs doivent etre comprise entre 20 et 144 Mhz
void
FobCommand::SetMeasureRateCount(unsigned short rate)
{
	unsigned char commande[4];
	unsigned short xmtr_time_count;

	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_RATEC;

	xmtr_time_count = (unsigned)(((crystalfreq/8.0)*1000.0)*
			  (((1000.0/rate)-0.3)/4.0));

	commande[2] = (unsigned char)(xmtr_time_count& 0x0ff);
	commande[3]= (unsigned char)((xmtr_time_count & 0x0ff00) >> 8);
	Put(commande,4);
}

void 
FobCommand::GetMeasureRate()
{

	unsigned char commande[2];
	unsigned char buffer[2];
	unsigned short i;
	

	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_RATE;
	Put(commande,2);
	intB->get(buffer,2);	

	i = ((*(unsigned char *)&buffer[0]) & 0xff)
		    + (((*(unsigned char *)&buffer[1]) & 0xff) << 8) ;
	
	printf("Transmitter Time Counts: %u\n\r",i);
}

void
FobCommand::SetMeasureRate(unsigned short rate)
{

	unsigned char commande[4];

	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_RATE;
	rate*=256;
	commande[2] = (unsigned char)(rate& 0x0ff);
	commande[3]= (unsigned char)((rate & 0x0ff00) >> 8);
	Put(commande,4);
}

void 
FobCommand::GetDataReadyMode()
{


	unsigned char commande[2];
	unsigned char buffer[1];

	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_DATARM;
	Put(commande,2);
	intB->get(buffer,1);	

	printf ("\n\rData Ready Output is ");
	if (buffer[0] == TRUE)
		printf("ENABLED");
	else
		printf("DISABLED");
}

void
FobCommand::SetDataReadyMode(FobActive readymode)
{
	unsigned char commande[3];
	
	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_DATARM;
	commande[2]=readymode;
	Put(commande,3);
}

void 
FobCommand::GetDataReadyValue()
{
	unsigned char commande[2];
	unsigned char buffer[1];
	
	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_DATARV;
	Put(commande,2);
	intB->get(buffer,1);
	
	printf ("\n\rData Ready Output Character is %u (decimal), <%c> (ASCII)",(unsigned char) buffer[0],(char)buffer[0]);
}

void
FobCommand::SetDataReadyValue(unsigned char readychar)
{
	unsigned char commande[3];
	
	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_DATARV;
	commande[2]=readychar;
	Put(commande,3);
}


void 
FobCommand::GetError()
{
	unsigned char commande[2];
	unsigned char buffer[1];
	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_ERROR;
	Put(commande,2);
	intB->get(buffer,1);
	switch (buffer[0])
	{
		case 0:
		    printf("...No Errors Have Occurred");break;
		case 1:
		    printf("...System RAM Test Error");break;
		case 2:
		    printf("...Non-Volatile Storage Write Failure");break;
		case 3:
		    printf("...System EEPROM Configuration Corrupt");break;
		case 4:
		    printf("...Transmitter EEPROM Configuration Corrupt");break;
		case 5:
		    printf("...Receiver EEPROM Configuration Corrupt");break;
		case 6:
		    printf("...Invalid RS232 Command");break;
		case 7:
		    printf("...Not an FBB Master");break;
		case 8:
		    printf("...No 6DFOBs are Active");break;
		case 9:
		    printf("...6DFOB has not been Initialized");break;
		case 10:
		    printf("...FBB Receive Error - Intra Bird Bus");break;
		case 11:
		    printf("...RS232 Overrun and/or Framing Error");break;
		case 12:
		    printf("...FBB Receive Error - FBB Host Bus");break;
		case 13:
		    printf("...No FBB Command Response from Device");break;
		case 14:
		    printf("...Invalid FBB Host Command");break;
		case 15:
		    printf("...FBB Run Time Error");break;
		case 16:
		    printf("...Invalid CPU Speed");break;
		case 17:
		    printf("...Slave No Data Error");break;
		case 18:
		    printf("...Illegal Baud Rate");break;
		case 19:
		    printf("...Slave Acknowledge Error");break;
		case 20:
		    printf("...CPU Overflow Error - call factory");break;
		case 21:
		    printf("...Array Bounds Error - call factory");break;
		case 22:
		    printf("...Unused Opcode Error - call factory");break;
		case 23:
		    printf("...Escape Opcode Error - call factory");break;
		case 24:
		    printf("...Reserved Int 9 - call factory");break;
		case 25:
		    printf("...Reserved Int 10 - call factory");break;
		case 26:
		    printf("...Reserved Int 11 - call factory");break;
		case 27:
		    printf("...Numeric CPU Error - call factory");break;
		case 28:
		    printf("...CRT Syncronization Error");break;
		case 29:
		    printf("...Transmitter Not Active Error");break;
		case 30:
		    printf("...ERC Extended Range Transmitter Not Attached Error");break;
		case 31:
		    printf("...CPU Time Overflow Error");break;
		case 32:
		    printf("...Receiver Saturated Error");break;
		case 33:
		    printf("...Slave Configuration Error");break;
		case 34:
		    printf("...ERC Watchdog Error");break;
		case 35:
		    printf("...ERC Overtemp Error");break;
		default:
		    printf("...UNKNOWN ERROR... check user manual");break;
	}
}

void 
FobCommand::GetErrorMask()
{
	unsigned char commande[2];
	unsigned char buffer[1];
	
	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_ERRORM;
	Put(commande,2);
	intB->get(buffer,1);	
	
	 /* ON ERROR Mask */
	switch(buffer[0] & 0x3)
		{
			    case 0:   /* FATAL errors are Blinked Forever */
				printf ("\n\rFATAL Errors are Blinked Forever and Operation is Halted\n\r");
				break;
			    case 1:   /* FATAL errors are Blinked Once */
				printf ("\n\rFATAL Errors are Blinked Once and Operation is Resumed \n\r");
				break;
			    case 2:   /* FATAL errors are Not Blinked  */
				printf ("\n\rFATAL Errors are Not Blinked and Operation is Halted\n\r");
				break;
			    case 3:   /* FATAL errors are Not Blinked Forever */
				printf ("\n\rFATAL Errors are Not Blinked and Operation Continues\n\r");
				break;
		}
}

void
FobCommand::SetErrorMask(FobErrorMask mask)
{
	unsigned char commande[3];
	
	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_ERRORM;
	commande[2]=mask;
	Put(commande,3);
}

void 
FobCommand::GetFilterTableVm()
{
	unsigned char commande[2];
	unsigned char buffer[14];
	int i;

	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_FTABLEVM;
	Put(commande,2);
	intB->get(buffer,14);
	/* on ne gere pas le mode etendu dcfiltermsg = bird_dcfiltermsg*/
	
	for (i=0;i<7;i++)
	{
	    printf ("\rFor the range %s, current value is %u (decimal)\n\r",
			    bird_dcfiltermsg[i],
				    (unsigned short)((*(unsigned char *)&buffer[i*2]) +
				    ((*(unsigned char *)&buffer[i*2+1]) << 8)));

	}
}

/* s'assurer que  0<=Tv[i]<=32767 */
void 
FobCommand::SetFilterTableVm(unsigned short TVm[7])
{
	unsigned char commande[16];
	int i,j=2;

	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_FTABLEVM;
	
	for (i=0;i<7;i++)
	{	
		commande[j++] = (unsigned char) (TVm[i] & 0x0ff);
		commande[j++]= (unsigned char) ((TVm[i] & 0x0ff00) >> 8);
	}
	Put(commande,16);
}

void 
FobCommand::GetFilterTableAMax()
{
	unsigned char commande[2];
	unsigned char buffer[14];
	int i;
	
	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_FTABLEAM;
	Put(commande,2);
	intB->get(buffer,14);
	
	/* mode etendu dcfiltermsg = bird_dcfiltermsg */
	for (i=0; i<7; i++)
	{
		printf ("For the range %s, Filter constant is %6.4f\n\r",
			    bird_dcfiltermsg[i],
			    (float)(((*(unsigned char *)&buffer[i*2]) +
				    ((*(unsigned char *)&buffer[i*2+1]) << 8))
				     * WTF));
	}
}


/* s'assurer que  ALPHA_MIN<0..008<=Talpha_min[i]<=0.99996<ALPHA_MAX */
void
FobCommand::SetFilterTableAMax(float Talpha_max[7])
{
	unsigned char commande[16];
	int i,j=2;

	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_FTABLEAM;
	
	for (i=0;i<7;i++)
	{	
		commande[j++] = (unsigned char) ((short) (Talpha_max[i] * FTW) & 0x0ff);
		commande[j++]= (unsigned char) (((short) (Talpha_max[i] * FTW) & 0x0ff00) >> 8);
	}
	Put(commande,16);
}

void 
FobCommand::GetSuddenChangeMode()
{
	unsigned char commande[2];
	unsigned char buffer[1];
	
	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_SCHANGEM;
	Put(commande,2);
	intB->get(buffer,1);

	if (buffer[0] == TRUE)
		printf("Sudden Changes are not output");
	else
		printf("Sudden Changes are output");
}

void 
FobCommand::SetSuddenChangeMode(FobActive smode)
{
	unsigned char commande[3];
	
	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_SCHANGEM;
	commande[2]=smode;
	Put(commande,3);
}

void 
FobCommand::GetModel()
{
	unsigned char commande[2];
	unsigned char buffer[11];
	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_MODEL;
	Put(commande,2);
	intB->get(buffer,10);
	
	buffer[10] = '\0'; /* set the string termination */
	printf ("\n\rModel is: %s\n",&buffer[0]);
}


void 
FobCommand::GetExpanderError()
{
	unsigned char commande[2];
	unsigned char buffer[2];
	
	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_ERRORE;
	Put(commande,2);
	intB->get(buffer,2);	
}



void 
FobCommand::GetPositionReferenceFrame()
{
	unsigned char commande[2];
	unsigned char buffer[1];
	
	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_REFRAME;
	Put(commande,2);
	intB->get(buffer,1);	
	
	if (buffer[0] == TRUE)
		printf("XYZ Reference Frame defined\n");
	else
		printf("XYZ Reference Frame Not defined\n");
}

void
FobCommand::SetPositionReferenceFrame(FobActive rmode)
{
	unsigned char commande[3];
	
	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_REFRAME;
	commande[2]=rmode;
	Put(commande,3);
}

void 
FobCommand::GetResponseDelay()
{
	unsigned char commande[2];
	unsigned char buffer[2];
	
	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_RESDELAY;
	Put(commande,2);
	intB->get(buffer,2);	

	printf("\n\rFBB Host Response Delay is %d microseconds",
	(unsigned short)(((*(unsigned char *) &buffer[0]) +
	((*(unsigned char *) &buffer[1]) << 8))) * 10);
}


void
FobCommand::SetResponseDelay(char delay)
{

	unsigned char commande[3];

	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_RESDELAY;
	commande[2]=delay;
	Put(commande,3);
}

void
FobCommand::SetConfiguration()
{


}

void 
FobCommand::SetFBBArmed(FobActive  a)
{
	unsigned char commande[3];
	
	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_ARMED;
	commande[2]=a;
	Put(commande,3);
}



void
FobCommand::GetGroupMode()
{
	unsigned char commande[2];
	unsigned char buffer[1];
	
	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_GROUPM;
	Put(commande,2);
	intB->get(buffer,1);
	
	if (buffer[0] == TRUE)
		printf("\n\rGroup Data Mode is Enabled\n");
	else
		printf("\n\rGroup Data Mode is Disabled\n");
}

void 
FobCommand::SetGroupMode(FobActive  f)
{
	unsigned char commande[3];
	
	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_GROUPM;
	commande[2]=f;
	Put(commande,3);	
	GroupeMode=TRUE;
}

/* pas implemente en plein
tenir compte nombre de flock */

void
FobCommand::GetFlockConfig()
{
	unsigned char commande[2];
	unsigned char buffer[14];
	int i;
	
	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_FCONFIG;
	Put(commande,2);
	intB->get(buffer,14);
	
	/*    Display the Configuration for each Address */
	printf("\n\rFlock Configuration...");
	for (i=0;i<nb_flock;i++)
	    {
		printf("\n\rFBB ADDR %d: ",i+1);

		if (buffer[i] & 0x80)
		{
		    if (buffer[i] & 0x10)
			printf("ERC, ");
		    else
			printf("6DFOB, ");
		}
		else
		{
		    printf("NOT ACCESSIBLE");
		    continue;
		}

		if (buffer[i] & 0x40)
		    printf("RUNNING, ");
		else
		    printf("NOT RUNNING, ");

		if ((buffer[i] & 0x30) == 0x20)
		    printf("RCVR");
		if ((buffer[i] & 0x30) == 0x00)
		    printf("NO RCVR");

		if ((buffer[i] & 0x11) == 0x01)
		    printf(", XMTR ");
		else
		{
		   if ((buffer[i] & 0x10) == 0x00)
		       printf(", NO XMTR");
		}

		if ((buffer[i] & 0x11) == 0x11)
		    printf(" ERT 0");

		if ((buffer[i] & 0x12) == 0x12)
		    printf(" ERT 1");

		if ((buffer[i] & 0x14) == 0x14)
		    printf(" ERT 2");

		if ((buffer[i] & 0x18) == 0x18)
		    printf(" ERT 3");

		if ((buffer[i] & 0x1f) == 0x10)
		    printf(" NO ERTs");
	}
}

void 
FobCommand::GetConfiguration()
{


}

/* tient pas compte plusieurs flock 
nb flock nombre de flock avec ad #*/
void 
FobCommand::GetAutoConfiguration()
{
	unsigned char commande[2];
	unsigned char buffer[14];
	short i,j,active;
	
	commande[0]=B_EXAMINE_VALUE;
	commande[1]=BV_AUTOC;
	Put(commande,2);
	intB->get(buffer,5);	
	
	switch (buffer[0])
	    {
		case 0:
		    printf("\n\rFBB Bird configured for Stand Alone Operation\n\r");
		    break;
		case 1:
		    printf("\n\rFBB Bird configured for 1 Transmitter/N Receiver Operation\n\r");
		    break;
		default:
		    printf("** ERROR ** invalid FBB Configuration Mode\n\r");
	    }
	printf("\n\rFBB Devices..");
	    for (i=1;i<=nb_flock;i++)
	    {
		printf("\n\rDevice at FBB address %d is ", i);
		if (i < 15)
		{
		    j = *(unsigned char *)&buffer[1] +
			((*(unsigned char *)&buffer[2]) << 8);
		    active = ((0x1 << i) & j);
		}
		else
		{
		    j = *(unsigned char *)&buffer[5] +
			((*(unsigned char *)&buffer[6]) << 8);
		    active = ((0x1 << (i - 15)) & j);
		}

		if (active)
		   printf("RUNNING");
		else
		   printf("NOT RUNNING");
	    }
	    printf("\n\rFBB Dependents..");
	    for (i=1;i<=14;i++)
	    {
		printf("\n\rDevice at FBB address %d is ", i);
		j = *(unsigned char *)&buffer[3] +
		    ((*(unsigned char *)&buffer[4]) <<8);
		if ((0x1 << i) & j)
		   printf("DEPENDENT");
		else
		   printf("NOT DEPENDENT");
	    }
	    printf("\n");
}


void 
FobCommand::GetSync()
{
	unsigned char commande[2];
	unsigned char buffer[4];
	short i;

	commande[0]=B_SYNC;
	commande[1]=255;/* a mettre en constante */
	Put(commande,2);
	intB->get(buffer,4);	  
	for (i=0; i<4; i++)
	{		
		  //  Display the Retrace Voltage
		i = *(unsigned char *) buffer[0] +
		    ((*(unsigned char *) &buffer[1]) << 8);

		printf ("%6.4f  ", (float)((float)i * 5.0/32768.0));

		// Compute the Retrace Rate if the count is not = 0 ...then display
		
		i = *(unsigned char *)&buffer[2] +
		    ((*(unsigned char *)&buffer[3]) << 8);
		if (i != 0)
		     printf ("      %6.2f\n\r",
			  (float)((125000.0 * crystalfreq)/(unsigned short)i));
		else
		     printf ("       00.00\n\r");
	}
	SetSync(NO_SYNC);
}



void 
FobCommand::SetAutoConfiguration(int nb_flock)
{
	sleep(1);
	unsigned char commande[3];
	this->nb_flock = nb_flock;
	commande[0]=B_CHANGE_VALUE;
	commande[1]=BV_AUTOC;
	commande[2]=nb_flock;
	Put(commande,3);	
	sleep(1);
}

void 
FobCommand::GetConfig()
{
	GetStatus();
	GetRevNumber();
	GetCrystalFreq();
	GetPosScal();
	GetFilter();
	GetFilterTableAMin();
	GetMeasureRateCount();
	GetMeasureRate();
	GetDataReadyMode();
	GetDataReadyValue();
	GetError();
	GetErrorMask();
	GetFilterTableVm();
	GetFilterTableAMax();
	GetSuddenChangeMode();
	GetModel();
	GetExpanderError();
	GetPositionReferenceFrame();
	GetResponseDelay();
	GetGroupMode();
	GetFlockConfig();
	GetAutoConfiguration();
}
