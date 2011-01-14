

#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream.h>

#include "RS232Interface.h"

int
main (int argc, char *argv[],char *envp[])
{
  int            n;
  unsigned char  buffer[1024];
  
  int            listen = 1;

  RS232Interface i("/dev/ttyS0", B9600, CONTROL_HARD);

  i.connect ();
  i.clearBuffer ();

  if ( listen ) {
    while (1) {
      printf ("waiting for data...");
      fflush (stdout);
      while ( (n = i.getNumberData ()) == 0 ) {
        printf (".");
        fflush (stdout);
        sleep (1);
      };
      printf ("data has arrived (%i bytes): ", n);
      fflush (stdout);
      i.get(buffer, n);
      buffer[n] = '\0';
      printf (">>%s<<\n", buffer);
      fflush (stdout);
    };
  } else {
    while (1) {
      sprintf ((char*)buffer, "coucou");
      printf ("sending message >>%s<< (%i bytes)...\n", buffer, strlen((char*)buffer));
      fflush (stdout);
      i.put (buffer, strlen((char*)buffer));

      printf ("waiting for data...");
      fflush (stdout);
      while ( (n = i.getNumberData ()) == 0 ) {
        printf (".");
        fflush (stdout);

        printf ("sending message >>%s<< (%i bytes)...\n", buffer, strlen((char*)buffer));
        fflush (stdout);
        i.put (buffer, strlen((char*)buffer));

        sleep (1);
      };
      printf ("data has arrived (%i bytes): ", n);
      fflush (stdout);
      i.get(buffer, n);
      buffer[n] = '\0';
      printf (">>%s<<\n", buffer);
      fflush (stdout);
    };
    
  }

  return 0;
}
