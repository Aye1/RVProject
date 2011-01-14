
#include "ul.h"

/* ONLY COMPILE THIS FILE FOR MAC-OS/X SYSTEMS */

#ifdef UL_MAC_OSX

#include "pw.h"

void defaultExitFunc ()
{
  pwCleanup () ;
  exit ( 0 ) ;
}

void pwSetCallbacks ( pwKeybdFunc    *kb, pwMouseFunc *ms,
                      pwMousePosFunc *mp, pwResizeCB *rcb,
                      pwExitCB *ecb )
{
  fprintf ( stderr, "PW: ***** NOT IMPLEMENTED FOR OS-X YET!! *****\n" ) ;
}

void pwInit ( int multisample, int num_samples )
{
  /* Open a full-screen window here please. */
  fprintf ( stderr, "PW: ***** NOT IMPLEMENTED FOR OS-X YET!! *****\n" ) ;
}


void pwInit ( int x, int y, int w, int h, int multisample,
              char *title, int border, int num_samples )
{
  /* Open a regular window here please. */
  fprintf ( stderr, "PW: ***** NOT IMPLEMENTED FOR OS-X YET!! *****\n" ) ;
}


void pwGetSize ( int *w, int *h )
{
}


void pwSetCursor ( int c )
{
}


void pwSetSize ( int w, int h )
{
}


void pwSetOrigin ( int x, int y )
{
}


void pwSetSizeOrigin ( int x, int y, int w, int h )
{
}


void pwSwapBuffers ()
{
}


void pwCleanup ()
{
}


#endif

