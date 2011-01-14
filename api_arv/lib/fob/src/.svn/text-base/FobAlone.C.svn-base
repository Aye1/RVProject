#include <iostream>
#include "FobAlone.h"
#include "DriverFob.h"



FobAlone::FobAlone(Fob_RData type,Fob_HData hemisphere,const char
*device_file,RS232_Mode mode_flux,int frequency):FobCommand(device_file,mode_flux,frequency)
{
	mode=type;
	hem=hemisphere;
	scale_x=scale_y=scale_z=1.0;
	pos_x=pos_y=pos_z=0.0;
}

void
FobAlone::Init()
{
		
    init();

    // on se place dans le modele graphique de base x a droite y vers le haut z vers nous
    // cerr<<"set position reference frame"<<endl; 
    SetPositionReferenceFrame(ON);

    // cerr<<"set  reference frame"<<endl; 
    SetReferenceFrame(-90.0,0.0,-90.0);

    // cerr<<"set hemisphere"<<endl;  
   SetHemisphere(RotateHem(hem));
   // cerr<<"wait.."<<endl;

   // cerr<<"set init angle align"<<endl; 
    //on a align le repere du recepteur avec le nouveau repere du transmetteur
    SetAngleAlign(180.0,-90.0,-90.0);

    // sleep(1);
    
    //on fixe le type de donnee
    // cerr<<"set data type"<<endl; 
   SetRData(mode);
}

void
FobAlone::InitAlone()
{
	Init();
	//SetOrientationCenter();
	sleep(1);
}

void
FobAlone::InitSlave()
{//rem:mm corps de methode que InitAlone() mais peut etre modifie 
	
	Init();
	//SetOrientationCenter();
	sleep(1);
}

void
FobAlone::InitSlaveUnSeulRS232(int numero_slave)
{
	// On reprend le code de Init mais on prÃ©cise qu'on parle Ã 
	// l'esclave Ã  chaque fois

	// on se place dans le modele
	// graphique de base x a droite y vers le haut z vers nous
	// cerr<<"set position reference frame"<<endl; 
	ToFbb(numero_slave);
	SetPositionReferenceFrame(ON);
	
	// cerr<<"set  reference frame"<<endl; 
	ToFbb(numero_slave);
	SetReferenceFrame(-90.0,0.0,-90.0);
	
	// cerr<<"set hemisphere"<<endl;  
	ToFbb(numero_slave);
	SetHemisphere(RotateHem(hem));
	// cerr<<"wait.."<<endl;
	
	// cerr<<"set init angle align"<<endl; 
	//on a align le repere du recepteur avec le nouveau repere du transmetteur
	ToFbb(numero_slave);
	SetAngleAlign(180.0,-90.0,-90.0);
	
	// sleep(1);
    
	//on fixe le type de donnee
	// cerr<<"set data type"<<endl; 
	ToFbb(numero_slave);
	SetRData(mode);
}



void
FobAlone::InitMaster(int nb_flock)
{
	Init();
	SetAutoConfiguration(nb_flock);
	//SetOrientationCenter();
	
}

Fob_HData 
FobAlone::RotateHem(Fob_HData h)
{

	Fob_HData res;
	
	switch (h)
	{
 		case FORWARD:res=LOWER;axis_1=0;axis_2=1;break;
 		case AFT:res=UPPER;axis_1=0;axis_2=1;break;
 		case UPPER:res=RIGHT;axis_1=0;axis_2=2;break;
 		case LOWER:res=LEFT;axis_1=0;axis_2=2;break;
 		case LEFT:res=AFT;axis_1=1;axis_2=2;break;
 		case RIGHT:res=FORWARD;axis_1=1;axis_2=2;break;
	}
	return res;
}

void
FobAlone::Config()
{

}


// fixe l'echelle des valeurs de position retournes par le FOB
void 
FobAlone::SetScale(float x,float y,float z)
{
	scale_x=x;
	scale_y=y;
	scale_z=z;
}

void 
FobAlone::SetPosition(float x,float y,float z)
{
	pos_x=x;
	pos_y=y;
	pos_z=z;
} 

void
FobAlone::IncrPosition(float x,float y,float z)
{
	pos_x+=x;
	pos_y+=y;
	pos_z+=z;
}

void 
FobAlone::SetOrientation(float azimuth,float elevation,float roll)
{
    SetAngleAlign(azimuth,elevation,roll);
    sleep(1);
}


void 
FobAlone::Reset()
{
  //reinitialise le repere du transmetteur
	
	SetRData(POSITION_ANGLES);
	
	SetPositionReferenceFrame(ON);
	SetReferenceFrame(-90.0,0.0,-90.0);
		
	//reinitialise le chgt de repere entre transmetteur et recepteur
	SetAngleAlign(180.0,-90.0,-90.0);
	
	pos_x=pos_y=pos_z=0.0;
	scale_x=scale_y=scale_z=1.0;
}

void 
FobAlone::SetOrientationCenter()
{
    float angles[3];
    float pos[3];
    
    SetRData(POSITION_ANGLES);
    Point();
    FobCommand::GetPosAngle(pos,angles);
    std::cerr << angles[0] << " " << angles[1] << " " << angles[2] << std::endl;
    //  SetAngleAlign(angles[0],angles[1],angles[2]);
    
    SetRData(mode);
}

void 
FobAlone::SetPositionCenter()
{
    float pos[3];
    
    SetRData(POSITION);
    Point();
    FobCommand::GetPos(pos);
    
    pos_x=pos_x-pos[0]*ITCM*scale_x;
    pos_y=pos_y-pos[1]*ITCM*scale_y;
    pos_z=pos_z-pos[2]*ITCM*scale_z;
    SetRData(mode);
}


// fonctionnement en mode point

void 
FobAlone::GetPos(float pos[3])
{
    float post[3];
    
    FobCommand::Point();
    FobCommand::GetPos(post);
    
    pos[0]=post[0]*ITCM*scale_x+pos_x;
    pos[1]=post[1]*ITCM*scale_y+pos_y;
    pos[2]=post[2]*ITCM*scale_z+pos_z;
}

void
FobAlone::GetAngle(float angle[3])
{
    float anglet[3];
    
    FobCommand::Point();
    FobCommand::GetAngle(anglet);
    angle[0]=anglet[2];
    angle[1]=anglet[1];
    angle[2]=anglet[0];
}

void 
FobAlone::GetMatrix(float matrix[16])
{
    float matrixt[9];
    
    FobCommand::Point();
    FobCommand::GetMatrix(matrixt);
    
    matrix[0]=matrixt[7];matrix[1]=matrixt[8];
    matrix[2]=-matrixt[6];matrix[3]=0;
    
    matrix[4]=matrixt[4];matrix[5]=matrixt[5];
    matrix[6]=-matrixt[3];matrix[7]=0;
    
    matrix[8]=-matrixt[1];matrix[9]=-matrixt[2];
    matrix[10]=matrixt[0];matrix[11]=0;
    
    matrix[12]=0.0;matrix[13]=0.0;
    matrix[14]=0.0;matrix[15]=1.0;
}

void
FobAlone::GetQuaternion(float quat[4])
{
    float quatt[4];
    
    FobCommand::Point();
    FobCommand::GetQuaternion(quatt);
    
    quat[0]=quatt[1];
    quat[1]=quatt[2];
    quat[2]=quatt[3];
    quat[3]=-quatt[0];
}


void 
FobAlone::GetPosAngle(float pos[3],float angle[3])
{
    float post[3],anglet[3];
    
    Point();
    FobCommand::GetPosAngle(post,anglet);
 
    pos[0]=post[0]*ITCM*scale_x+pos_x;
    pos[1]=post[1]*ITCM*scale_y+pos_y;
    pos[2]=post[2]*ITCM*scale_z+pos_z;
    angle[0]=anglet[2];
    angle[1]=anglet[1];
    angle[2]=anglet[0];
}

void 
FobAlone::GetPosMatrix(float pos[3],float matrix[16])
{
    float post[3],matrixt[9];
    
    FobCommand::Point();
    FobCommand::GetPosMatrix(post,matrixt);
    
    pos[0]=post[0]*ITCM*scale_x+pos_x;
    pos[1]=post[1]*ITCM*scale_y+pos_y;
    pos[2]=post[2]*ITCM*scale_z+pos_z;
    
    matrix[0]=matrixt[0];matrix[1]=matrixt[3];
    matrix[2]=matrixt[6];matrix[3]=0;
    
    matrix[4]=matrixt[1];matrix[5]=matrixt[4];
    matrix[6]=matrixt[7];matrix[7]=0;
    
    matrix[8]=matrixt[2];matrix[9]=matrixt[5];
    matrix[10]=matrixt[8];matrix[11]=0;
    
    matrix[12]=0.0;matrix[13]=0.0;
    matrix[14]=0.0;matrix[15]=1.0;
}

void 
FobAlone::GetPosQuaternion(float pos[3],float quat[4])
{
    float post[3];
    float quatt[4];
    
    FobCommand::Point();
    FobCommand::GetPosQuaternion(post,quatt);
    
    pos[0]=post[0]*ITCM*scale_x+pos_x;
    pos[1]=post[1]*ITCM*scale_y+pos_y;
    pos[2]=post[2]*ITCM*scale_z+pos_z;
    quat[0]=quatt[1];
    quat[1]=quatt[2];
    quat[2]=quatt[3];
    quat[3]=-quatt[0];
}


void 
FobAlone::GetMatrixGL(float matrix[16])
{
    float matrixt[9],post[3];
    
    FobCommand::Point();
    FobCommand::GetPosMatrix(post,matrixt);
    
    matrix[0]=matrixt[0];matrix[1]=matrixt[3];
    matrix[2]=matrixt[6];matrix[3]=0;
    
    matrix[4]=matrixt[1];matrix[5]=matrixt[4];
    matrix[6]=matrixt[7];matrix[7]=0;
    
    matrix[8]=matrixt[2];matrix[9]=matrixt[5];
    matrix[10]=matrixt[8];matrix[11]=0;
    
    matrix[12]=post[0]*scale_x+pos_x;matrix[13]=post[1]*scale_y+pos_y;
    matrix[14]=post[2]*scale_z+pos_z;matrix[15]=1.0;
    
    
}

void
FobAlone::RunDriver()
{
// 	pthread_attr_t attribut;
	SetRData(POSITION_QUATERNION);
// 	
// 	pthread_attr_init(&attribut);
// 	//pthread_attr_setscope(&attribut,PTHREAD_SCOPE_PROCESS); 
// 	pthread_attr_setscope(&attribut,PTHREAD_SCOPE_SYSTEM);//real time !
// 	//pthread_attr_setschedpolicy();
// 	//pthread_attr_setschedparam();
 	//pthread_create(&fob_thread,&attribut,DriverFob,(void *)this);
// 	
 	pthread_create(&fob_thread, NULL,DriverFob,(void *)this);
 	pthread_mutex_init(&mutex,NULL);
 	sleep(1);//on attend que le thread se mettent en route on peut faire un join
    
    Stream();
}

void
FobAlone::StopDriver()
{
	//arret du mode stream
    Point();
    //on termine le thread d'ecoute du port 
    pthread_cancel(fob_thread);//pan ! pan ! 
}

void 
FobAlone::GetSPosQuaternion(float pos[3],float quat[4])
{
 	pthread_mutex_lock(&mutex);
    //le lock se fait avec FobPerf
    pos[0]=shared_pos[0]*ITCM*scale_x+pos_x;
    pos[1]=shared_pos[1]*ITCM*scale_y+pos_y;
    pos[2]=shared_pos[2]*ITCM*scale_z+pos_z;
    quat[0]=shared_quat[1];
    quat[1]=shared_quat[2];
    quat[2]=shared_quat[3];
    quat[3]=-shared_quat[0];
 	pthread_mutex_unlock(&mutex);
// 
}

