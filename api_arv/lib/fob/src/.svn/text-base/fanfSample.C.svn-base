#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream.h>

#include "FobAlone.h"

int
main (int argc, char *argv[],char *envp[])
{
    int c;
    float pos[3];
    
    printf ("Creation et construction de l'objet FobAlone...\n");
    c = getchar ();
    FobAlone  myFob (POSITION_ANGLES, FORWARD,"/dev/ttyS0", CONTROL_HARD);

    printf ("Initialisation en mode Alone...\n");
    c = getchar ();
    myFob.InitAlone ();

    printf ("Enregistre la position courante comme origine...\n");
    c = getchar ();
    myFob.SetOrientationCenter ();
    myFob.SetPositionCenter ();

    do {
      printf ("Lit une position (q pour quit)...\n");
      c = getchar ();
      if ((c != 'q') && (c != 'Q')) {
        myFob.GetPos (pos);
        printf ("%10.5f %10.5f %10.5f\n", pos[0], pos[1], pos[2]);
      }
    } while ((c != 'q') && (c != 'Q'));

    printf ("Quitting...\n");
    c = getchar ();

    return 0;
    
}
