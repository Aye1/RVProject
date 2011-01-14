
int Signe(float valeur)
{
	if (valeur<0.0)
		return -1;
	else
		return 1;
}


static void* DriverFob(void*data)
{

    float pos[3],quat[4];
    float posf[3],quatf[4];
    float signe;
    float lastPos[3];

    FobAlone* fob=(FobAlone*)data;

    cerr<<"NOTIFY->ALONEFOB/RUN:start tracking process.."<<endl;

    while (1)
    {	
	//get value
	  fob->FobCommand::GetPosQuaternion(pos,quat);
	  //hemisphere change test
	  if ((Signe(lastPos[fob->axis_1])==-Signe(pos[fob->axis_1]))
	      &&(Signe(lastPos[fob->axis_2])==-Signe(pos[fob->axis_2])))     
	    signe=-signe;
	  
	//update hemisphere
	posf[0]=signe*pos[0];
	posf[1]=signe*pos[1];
	posf[2]=signe*pos[2];				
	quatf[0]=quat[0];
	quatf[1]=quat[1];
	quatf[2]=quat[2];
	quatf[3]=quat[3];      	
	//pass to shared buffer
	pthread_mutex_lock(&(fob->mutex));
	fob->shared_pos[0]=posf[0];
	fob->shared_pos[1]=posf[1];
	fob->shared_pos[2]=posf[2];
	fob->shared_quat[0]= quatf[0];
	fob->shared_quat[1]= quatf[1];
	fob->shared_quat[2]= quatf[2];
	fob->shared_quat[3]= quatf[3];
	pthread_mutex_unlock(&(fob->mutex));
	
	//update last value
	lastPos[0]=pos[0];
	lastPos[1]=pos[1];
	lastPos[2]=pos[2];      
    }

}
