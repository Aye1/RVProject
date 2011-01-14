#include <iostream>
#include "RS232Interface.h"
using namespace std;
//use rs232 posix interface

RS232Interface::RS232Interface(const char *comm,long baudspeed,RS232_Mode mode)
{
    com_port=new char[strlen(comm)+1];
    strcpy(com_port,comm);
    baudrate=baudspeed;
    mode_flux=mode;
}

void
RS232Interface::clearBuffer()
{
    tcflush(serial_handle,TCIFLUSH);
}

void
RS232Interface::connect()
{
  // cerr<<"attend ici"<<endl;
    if ((serial_handle=open(com_port,O_RDWR))==-1) //on ouvre un descripteur de fichier
    {
	cerr<<"ERROR->RS232INTERFACE/CONNECT: can't connect to "<<com_port<<endl;
	exit(1);
    }
    // cerr<<"apres ici"<<endl;

    tcgetattr(serial_handle,&old_termio);//on sauvegarde l'ancienne structure
//     baudrate=cfgetospeed(&com_termio);  /* output */
//     cerr<<"actual in speed="<<baudrate<<endl;
//     baudrate=cfgetispeed(&com_termio);  /* input */    
    
//     cerr<<"actual out speed="<<baudrate<<endl;

    // on definit la vitesse 
    cfsetospeed(&com_termio, baudrate);  /* output */
    cfsetispeed(&com_termio, baudrate);  /* input */
     
    baudrate=cfgetospeed(&com_termio);  /* output */
    // cerr<<"actual in speed="<<baudrate<<endl;
    baudrate=cfgetispeed(&com_termio);  /* input */    
    
    // cerr<<"actual out speed="<<baudrate<<endl;

    //on definit les options
    
    if (mode_flux==CONTROL_HARD)
    {
	com_termio.c_iflag = 0;
	// 8 bits /connection direct/ le receveur est enclenche / controle HARD
//	com_termio.c_cflag = CS8 | CLOCAL | CREAD| CNEW_RTSCTS;
	com_termio.c_cflag = CS8 | CLOCAL | CREAD| CRTSCTS;
	com_termio.c_lflag = 0;
	com_termio.c_cc[VMIN] = 0;
	com_termio.c_cc[VTIME] = 20;
    }
    else //controle de flux soft
    { 
      //  cerr<<"mode soft"<<endl;

      //int control;
      // ioctl(serial_handle, TIOCMGET, &control);  
      //  control &= ~(TIOCM_RTS);            
      //  ioctl(serial_handle, TIOCMSET, &control);  
      //  sleep(1);
	com_termio.c_iflag = IXOFF;//controle soft
	// 8 bits /connection direct/ le receveur est enclenche /
	com_termio.c_cflag = CS8 | CLOCAL | CREAD;
	com_termio.c_lflag = 0;
	com_termio.c_cc[VMIN] = 0;
	com_termio.c_cc[VTIME] = 20;/*return after 2 second */
    }
    
    tcsetattr(serial_handle, TCSANOW, &com_termio); //associe la structure
    sleep(4);
    tcflush(serial_handle,TCIOFLUSH);//vide le buffer entree & sortie
}

void
RS232Interface::disconnect()
{
    
    if (close(serial_handle)==-1) //on ferme le port
    {
	cerr<<"ERROR->RS232INTERFACE/DISCONNECT: can't close the port"<<endl;
	exit(1);
    }
    tcsetattr(serial_handle, TCSANOW, &old_termio); //on remet l'ancienne configuration
}

int
RS232Interface::put(unsigned char *data,int nb_bytes)
{
    
    int nb_write=0;//nombre d'octets envoyes
    
    nb_write=write(serial_handle,data,nb_bytes);
    sleep(1);
    tcflush(serial_handle,TCOFLUSH);//on vide le buffer de sortie
    if (nb_write<nb_bytes) //on a pas reussi a envoyer tous les caracteres
    {
	cerr<<"ERROR->RS232INTERFACE/PUT : put  "<<nb_write<<" byte /"<<nb_bytes<<endl;
	return (0);
    }
    else
	return (1);
}

int
RS232Interface::put_c(unsigned char data)
{

    int nb_write=0;//nombre d'octets envoyes
    
    nb_write=write(serial_handle,&data,1);
    usleep(50);
    tcflush(serial_handle,TCOFLUSH);
    if (nb_write<1) //on a pas reussi a envoyer la commande
    {	
	cerr<<"Erreur: on n'a pas envoye la commande\n";
	return (0);
    }
    else	
	return (1);
}


int 
RS232Interface::getNumberData()
{
    int nb_available;

    ioctl(serial_handle,FIONREAD,&nb_available);
    return nb_available;
}

int
RS232Interface::get(unsigned char *data,int nb_bytes)
{

    //size_t nb_available;
    int nb_read=1;//nombre de bytes lu (initialise a 1 pour rentre dans la boucle )
    int nb_read_t=0;//nombre total de bytes lu
    
    /* la boucle nous permet de nous assurer qu'on lit tous les
       caracteres recues dans le buffer.A des vitesses eleves, la lecture est
       effectue en une seule fois.Si 0 caracteres sont renvoyes on a une erreur donc
       on sort. */
//     if (ioctl(serial_handle,FIONREAD,&nb_available)<0)
//     {
//   	cerr<<"Erreur: ioctl retourne une erreur\n";
//   	return (0);
//     }
//     else
//   	cerr<<"nb de caractere disponible :"<<nb_available<<endl;
//    
    
    while ((nb_read_t<nb_bytes)&&(nb_read!=0))
    {
	nb_read=read(serial_handle,&data[nb_read_t],nb_bytes-nb_read_t);
	nb_read_t+=nb_read;
    }
    if (nb_read==0) //on a pas put recuperer tous les bytes
    {	
	cerr<<"Erreur: on n'a put recuperer que " <<nb_read_t<< " caracteres sur" <<nb_bytes<<"\n";
	return (0);
    }
    else		
	return (1);
}

