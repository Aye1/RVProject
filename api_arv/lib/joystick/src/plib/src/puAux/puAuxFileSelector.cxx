/*
     PLIB - A Suite of Portable Game Libraries
     Copyright (C) 1998,2002  Steve Baker

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Library General Public
     License as published by the Free Software Foundation; either
     version 2 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Library General Public License for more details.

     You should have received a copy of the GNU Library General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

     For further information visit http://plib.sourceforge.net

     $Id: puAuxFileSelector.cxx,v 1.1 2004/02/28 00:54:03 sjbaker Exp $
*/

/****
* NAME
*   puaFileSelector
*
* DESCRIPTION
*   PUI dialog for selecting files
*
* AUTHOR
*   Dave McClurg <dpm@efn.org>
*
* CREATION DATE
*   August 2000
*
* MODIFICATION HISTORY
*   John Fay    - many improvements
*   Steve Baker - moved from FilePicker to FileSelector due to
*                 key API change.
****/


#include "puAux.h"

UL_RTTI_DEF1(puaFileSelector,puDialogBox)


#ifdef UL_MSVC
#define DOTDOTSLASH "..\\"
#define SLASH       "\\"
#else
#define DOTDOTSLASH "../"
#define SLASH       "/"
#endif
#define SLASH_LEN     1

static void puaFileSelectorHandleSlider ( puObject * slider )
{
  float val ;
  slider -> getValue ( &val ) ;
  val = 1.0f - val ;

  puListBox* list_box = (puListBox*) slider -> getUserData () ;
  int idx = int ( list_box -> getNumItems () * val ) ;
  list_box -> setTopItem ( idx ) ;
}

static void puaFileSelectorHandleArrow ( puObject *arrow )
{
  puSlider *slider = (puSlider *) arrow->getUserData () ;
  puListBox* list_box = (puListBox*) slider -> getUserData () ;

  int type = ((puArrowButton *)arrow)->getArrowType() ;
  int inc = ( type == PUARROW_DOWN     ) ?   1 :
            ( type == PUARROW_UP       ) ?  -1 :
            ( type == PUARROW_FASTDOWN ) ?  10 :
            ( type == PUARROW_FASTUP   ) ? -10 : 0 ;

  float val ;
  slider -> getValue ( &val ) ;
  val = 1.0f - val ;
  int num_items = list_box->getNumItems () - 1 ;
  if ( num_items > 0 )
  {
    int idx = int ( num_items * val + 0.5f ) + inc ;
    if ( idx > num_items ) idx = num_items ;
    if ( idx < 0 ) idx = 0 ;

    slider -> setValue ( 1.0f - (float)idx / num_items ) ;
    list_box -> setTopItem ( idx ) ;
  }
}


static bool is_slash ( char ch )
{
#ifdef UL_WIN32
  /* win32 can use back or forward slash */
  return ch == '\\' || ch == '/' ;
#else
  return ch == SLASH[0] ;
#endif
}


static void chop_file ( char *fname )
{
  /* removes everything back to the last '/' */

  for ( int i = strlen(fname)-1 ; ! is_slash ( fname[i] ) && i >= 0 ; i-- )
    fname[i] = '\0' ;
}


static void go_up_one_directory ( char *fname )
{
  /* removes everything back to the last but one '/' */

  chop_file ( fname ) ;

  if ( fname[0] == '\0' )
  {
    /* Empty string! We must be at the root. */

    strcpy ( fname, SLASH ) ;
    return ;
  }

  /* Otherwise, just delete the last element of the path. */

  /* Remove the trailing slash - then remove the rest as
     if it was a file name */

  fname [ strlen(fname)-1 ] = '\0' ;
  chop_file ( fname ) ;
}


void puaFileSelector::input_entered ( puObject* inp )
{
  puaFileSelector* file_selector = (puaFileSelector*) (inp -> getUserData ()) ;

  /*
    If the input selector is empty - do nothing
  */

  if ( inp -> getStringValue() [ 0 ] == '\0' )
    return ;

  if ( ulIsAbsolutePathName ( inp -> getStringValue () ) )
    file_selector -> setValue ( inp -> getStringValue () ) ;
  else
  {
    char *s = new char [   strlen ( file_selector->__getStartDir() )
                         + strlen ( SLASH )
                         + strlen ( inp -> getStringValue () )
                         + 1 ] ;

    ulMakePath ( s, file_selector -> __getStartDir(),
                 inp -> getStringValue () ) ;
    file_selector -> setValue ( s ) ;

    delete [] s ;
  }

  /*
    If he typed a valid directory name - then
    refresh the contents of the browser window
  */

  ulDir *d = ulOpenDir ( file_selector->getStringValue() ) ;
  ulCloseDir ( d ) ;

  if ( d != NULL )
    file_selector -> find_files () ;
}


void puaFileSelector::handle_select ( puObject* l_box )
{
  puaFileSelector* file_selector = (puaFileSelector*) l_box -> getUserData () ;

  int selected ;
  l_box -> getValue ( &selected ) ;

  if ( selected >= 0 && selected < file_selector -> num_files )
  {
    char *dst = file_selector -> getStringValue () ;
    char *src = file_selector -> files [ selected ] ;

    chop_file ( dst ) ;

    if ( strcmp ( src, "[..]" ) == 0 )
    {
      /* Do back up one level - so refresh. */

      go_up_one_directory ( dst ) ;
      file_selector -> find_files () ;
      file_selector -> __getInput() -> setValue ( dst ) ;
      return ;
    } 

    if ( file_selector -> dflag [ selected ] )
    {
      /* If this is a directory - then descend into it and refresh */

      src++ ; /* Remove leading '[' */

      int dst_len = strlen ( dst ) ;
      int src_len = strlen ( src ) ;
      char *new_dst = new char [ dst_len + src_len + 1 ] ;

      memcpy ( new_dst, dst, dst_len ) ;
      memcpy ( new_dst + dst_len, src, src_len + 1 ) ; /* Copy trailing '\0' */

      /* Replace trailing ']' with slash */
      new_dst [ dst_len + src_len - 1 ] = SLASH[0] ;

      file_selector -> __getInput() -> setValue ( new_dst ) ;
      file_selector -> setValue ( new_dst ) ;

      delete [] new_dst ;
      file_selector -> find_files () ;
      return ;
    }

    /* If this is a regular file - then just append it to the string */

    int dst_len = strlen ( dst ) ;
    int src_len = strlen ( src ) ;
    char *new_dst = new char [ dst_len + src_len + 1 ] ;

    memcpy ( new_dst, dst, dst_len ) ;
    memcpy ( new_dst + dst_len, src, src_len + 1 ) ; /* Copy trailing '\0' */

    file_selector -> __getInput() -> setValue ( new_dst ) ;
    file_selector -> setValue ( new_dst ) ;

    delete [] new_dst ;
  }
  else
  {
    /*
      The user clicked on blank screen - maybe we should
      refresh just in case some other process created the
      file.
    */

    file_selector -> find_files () ;
  }
}


static void puaFileSelectorHandleCancel ( puObject* b )
{
  puaFileSelector* file_selector = (puaFileSelector*) b -> getUserData () ;
  file_selector -> setValue ( "" ) ;
  file_selector -> invokeCallback () ;
}

static void puaFileSelectorHandleOk ( puObject* b )
{
  puaFileSelector* file_selector = (puaFileSelector*) b -> getUserData () ;

  file_selector -> invokeCallback () ;
}

void puaFileSelector::setSize ( int w, int h )
{
  // Resize the frame widget
  frame->setSize ( w, h ) ;

  // Resize and position the slider
  slider->setPosition ( w-30, 40+20*arrow_count ) ;
  slider->setSize ( 20, h-60-40*arrow_count ) ;

  // Position the arrow buttons
  if ( up_arrow )
  {
    up_arrow->setPosition ( w-30, h-20-20*arrow_count ) ;
    down_arrow->setPosition ( w-30, 20+20*arrow_count ) ;
  }

  if ( fastup_arrow )
  {
    fastup_arrow->setPosition ( w-30, h-40 ) ;
    fastdown_arrow->setPosition ( w-30, 40 ) ;
  }

  // Resize the list box
  list_box->setSize ( w-40, h-80 ) ;

  // Resoze and position the buttons
  cancel_button->setSize ( (w<170)?(w/2-15):70, 20 ) ;
  ok_button->setSize ( (w<170)?(w/2-15):70, 20 ) ;
  ok_button->setPosition ( (w<170)?(w/2+5):90, 10 ) ;
}

puaFileSelector::~puaFileSelector ()
{
  delete [] startDir ;

  if ( files != NULL )
  {
    for ( int i=0; i<num_files; i++ )
      delete [] files[i];

    delete [] files;
    delete [] dflag;
  }
}

void puaFileSelector::puaFileSelectorInit ( int x, int y, int w, int h, 
                                            int arrows,
                                            const char *dir,
                                            const char *title )
{
  type |= PUCLASS_FILESELECTOR ;

  files = NULL ;
  dflag = NULL ;
  num_files = 0 ;

  /* Character string lengths for efficiency in string processing */
  int start_dir_len ;
  int dir_len       = strlen ( dir ) ;

  if ( ulIsAbsolutePathName ( dir ) )
  {
    /* Include space for trailing slash in case it is necessary */
    startDir = new char [ dir_len + SLASH_LEN + 1 ] ;

    memcpy ( startDir, dir, dir_len ) ;
    start_dir_len = dir_len ;
  }
  else
  {
    char *cwd = new char [ UL_NAME_MAX + 1 ] ;
    if ( ulGetCWD ( cwd, UL_NAME_MAX + 1 ) == NULL )
    {
      ulSetError ( UL_WARNING,
                   "PUI: puaFileSelector - can't find current directory" ) ;
      setValue ( "" ) ;
      invokeCallback () ;
      delete [] cwd ;
      return ;
    }

    /* Include space for slashes in case they are necessary */
    int cwd_len = strlen ( cwd ) ;
    startDir = new char [   cwd_len
                          + 2 * SLASH_LEN
                          + dir_len + 1 ] ;

    memcpy ( startDir, cwd, cwd_len ) ;
    start_dir_len = cwd_len ;

    if ( ! is_slash ( cwd[cwd_len-1] ) )
    {
      memcpy ( startDir + cwd_len, SLASH, SLASH_LEN ) ;
      start_dir_len += SLASH_LEN ;
    }

    memcpy ( startDir + start_dir_len, dir, dir_len ) ;
    start_dir_len += dir_len ;

    delete [] cwd ;
  }

  if ( ! is_slash ( startDir[start_dir_len-1] ) )
  {
    memcpy ( startDir + start_dir_len, SLASH, SLASH_LEN ) ;
    start_dir_len += SLASH_LEN ;
  }

  startDir [ start_dir_len ] = '\0' ;
  setValue ( startDir ) ;

  if ( arrows > 2 ) arrows = 2 ;
  if ( arrows < 0 ) arrows = 0 ;
  arrow_count = arrows ;

  frame = new puFrame ( 0, 0, w, h );

  slider = new puSlider (w-30,40+20*arrows,h-60-40*arrows,TRUE,20);
  slider->setValue(1.0f);
  slider->setSliderFraction (0.2f) ;
  slider->setCBMode( PUSLIDER_DELTA );
  
  list_box = new puListBox ( 10, 60, w-40, h-20 ) ;
  list_box -> setLabel ( title );
  list_box -> setLabelPlace ( PUPLACE_TOP_LEFT ) ;
  list_box -> setStyle ( -PUSTYLE_SMALL_SHADED ) ;
  list_box -> setUserData ( this ) ;
  list_box -> setCallback ( handle_select ) ;
  list_box -> setValue ( 0 ) ;

  find_files () ;

  handle_select ( list_box ) ;

  slider -> setUserData ( list_box ) ;
  slider -> setCallback ( puaFileSelectorHandleSlider ) ;

  input = new puInput ( 10, 40, w-40, 60 ) ;
  input -> setValue ( startDir ) ;
  input -> setUserData ( this ) ;
  input -> setCallback ( input_entered ) ;
  input -> setDownCallback ( input_entered ) ;

  cancel_button = new puOneShot ( 10, 10, (w<170)?(w/2-5):80, 30 ) ;
  cancel_button -> setLegend ( "Cancel" ) ;
  cancel_button -> setUserData ( this ) ;
  cancel_button -> setCallback ( puaFileSelectorHandleCancel ) ;

  ok_button = new puOneShot ( (w<170)?(w/2+5):90, 10, (w<170)?(w-10):160, 30 ) ;
  ok_button -> setLegend ( "Ok" ) ;
  ok_button -> setUserData ( this ) ;
  ok_button -> setCallback ( puaFileSelectorHandleOk ) ;

  up_arrow = down_arrow = NULL ;
  fastup_arrow = fastdown_arrow = NULL ;

  if ( arrows > 0 )
  {
    down_arrow = new puArrowButton ( w-30, 20+20*arrows, w-10, 40+20*arrows, PUARROW_DOWN ) ;
    down_arrow->setUserData ( slider ) ;
    down_arrow->setCallback ( puaFileSelectorHandleArrow ) ;

    up_arrow = new puArrowButton ( w-30, h-20-20*arrows, w-10, h-20*arrows, PUARROW_UP ) ;
    up_arrow->setUserData ( slider ) ;
    up_arrow->setCallback ( puaFileSelectorHandleArrow ) ;
  }

  if ( arrows == 2 )
  {
    fastdown_arrow = new puArrowButton ( w-30, 40, w-10, 60, PUARROW_FASTDOWN ) ;
    fastdown_arrow->setUserData ( slider ) ;
    fastdown_arrow->setCallback ( puaFileSelectorHandleArrow ) ;

    fastup_arrow = new puArrowButton ( w-30, h-40, w-10, h-20, PUARROW_FASTUP ) ;
    fastup_arrow->setUserData ( slider ) ;
    fastup_arrow->setCallback ( puaFileSelectorHandleArrow ) ;
  }

  close  () ;
  reveal () ;
}

static int puaFileSelectorStringCompare ( const char *s1, const char *s2,
                                       const char  f1, const char  f2 )
{
  if ( f1 > f2 )    /* Directories before regular files. */
    return -1 ;

  if ( f1 < f2 )
    return 1 ;

  while ( 1 )
  {
    char c1 = s1? (*s1++): 0 ;
    char c2 = s2? (*s2++): 0 ;
    
    //end of string?
    if ( !c1 || !c2 )
    {
      if ( c1 )
        return 1 ; //s1 is longer
      if ( c2 )
        return -1 ; //s1 is shorter
      return 0 ;
    }
    
    if ( c1 == c2 )
      continue ;

    /*
      Windoze users are case-insensitive - so they presumably
      want case ignored in the sorting.

      UNIX users, however, do things like calling their makefiles
      'Makefile' so as to force them to the start of the directory
      listing.
    */

#ifdef UL_MSVC
    if ( c1 >= 'a' && c1 <= 'z' )
      c1 = c1 - ('a'-'A') ;
    
    if ( c2 >= 'a' && c2 <= 'z' )
      c2 = c2 - ('a'-'A') ;
#endif
    
    if ( c1 != c2 )
    {
      if ( c1 < c2 )
        return -1 ;
      return 1 ;
    }
  }
  return 0 ;
}


void puaFileSelector::setInitialValue ( const char *s )
{
  if ( ulIsAbsolutePathName ( s ) )
  {
    input -> setValue ( s ) ;
    input -> invokeCallback () ;
  }
  else
  {
    int str_val_len = strlen ( input->getStringValue () ) ;
    int s_len = strlen ( s ) ;
    char *s_new = new char [ str_val_len + s_len + 1 ] ;

    memcpy ( s_new, input->getStringValue (), str_val_len ) ;
    memcpy ( s_new + str_val_len, s, s_len + 1 ) ;  /* Plus one to get the final '\0' */

    input->setValue ( s_new ) ;
    delete [] s_new ;
    input -> invokeCallback () ;
  }
}


static void puaFileSelectorSort ( char** list, char *flags, int size )
//
//  comb sort - a modified bubble sort
//    taken from BYTE, April 1991, ppg 315-320
//
{
  int switches ;
  int gap = size ;

  do
  {
    gap = ((gap * 197) >> 8);  // gap /= 1.3;

    switch (gap)
    {
      case 0:  // the smallest gap is 1 -- bubble sort
	gap = 1;
	break;

      case 9:  // this is what makes this Combsort11
      case 10:
	gap = 11;
	break;
    }

    switches = 0 ; // dirty pass flag

    int top = size - gap;

    for ( int i=0; i<top; ++i )
    {
      int j=i+gap;

      if ( puaFileSelectorStringCompare ( list [i], list [j],
                                          flags[i], flags[j] ) > 0 )
      {
        char* temp = list[i] ;
        list[i] = list[j] ;
        list[j] = temp ;

        char tmp2 = flags[i] ;
        flags[i] = flags[j] ;
        flags[j] = tmp2 ;

        ++switches;
      }
    }
  } while ( switches || gap > 1 ) ;
}


void puaFileSelector::find_files ( void )
{
  if ( files != NULL )
  {
    for ( int i = 0 ; i < num_files ; i++ )
      delete [] files[i] ;

    delete [] files ;
    delete [] dflag ;
  }

  num_files = 0 ;

  int str_val_len = strlen ( getStringValue () ) ;

  if ( ! is_slash ( getStringValue () [ str_val_len - 1 ] ) )
  /* Someone forgot a trailing '/' */
  {
    char *new_dir = new char [ str_val_len + SLASH_LEN + 1 ] ;

    memcpy ( new_dir, getStringValue (), str_val_len ) ;
    memcpy ( new_dir + str_val_len, SLASH, SLASH_LEN + 1 ) ;  /* Plus one to include the trailing '\0' */
    setValue ( new_dir ) ;
    delete [] new_dir ;
  }

  int ifile = 0 ;

  char     *dir  = getStringValue () ;
  ulDir    *dirp = ulOpenDir ( dir ) ;
  ulDirEnt *dp ;

  if ( dirp == NULL )
  {
    perror ("puaFileSelector") ;
    ulSetError ( UL_WARNING, "PUI:puaFileSelector - can't open directory '%s'", dir ) ;
    num_files = 0 ;
    return ;
  }

  /* Count the number of files - skipping "." */

  while ( ( dp = ulReadDir(dirp) ) != NULL )
    if ( strcmp ( dp->d_name, "." ) != 0 )
      ifile++ ;

  ulCloseDir ( dirp ) ;

  num_files = ifile ;
  if ( num_files > 5 )
    slider -> setDelta ( 0.5f / num_files ) ;
  else
    slider -> setDelta ( 0.1f ) ;

  if ( num_files == 0 )
  {
    ulSetError ( UL_WARNING,
		   "PUI:puaFileSelector - no entries in directory '%s'?!", dir ) ;
    return ;
  }

  files = new char* [ num_files+1 ] ;
  dflag = new char  [ num_files+1 ] ;

  dirp = ulOpenDir ( dir ) ;

  if ( dirp == NULL )
  {
    perror ("puaFileSelector") ;
    ulSetError ( UL_WARNING,
                   "PUI:puaFileSelector - can't re-open directory '%s'", dir ) ;
    num_files = 0 ;
    return ;
  }

  ifile = 0 ;

  while ( (dp = ulReadDir(dirp)) != NULL && ifile < num_files )
  {
    /* Skip over the "." entry... */

    if ( strcmp ( dp->d_name, "." ) != 0 )
    {
      dflag[ ifile ] = dp->d_isdir ;

      int name_len = strlen ( dp->d_name ) ;
      if ( dflag[ ifile ] )
      {
        files[ ifile ] = new char[ name_len + 3 ] ;

        files [ ifile ] [ 0 ] = '[' ;
        memcpy ( files [ ifile ] + 1, dp->d_name, name_len ) ;
        strcpy ( files [ ifile ] + 1 + name_len, "]" ) ;
      }
      else
      {
        files[ ifile ] = new char[ name_len + 1 ] ;
        memcpy ( files [ ifile ], dp->d_name, name_len + 1 ) ;  /* Plus one to include the final '\0' */
      }

      ifile++ ;
    }
  }

  files [ ifile ] = NULL ;

  ulCloseDir ( dirp ) ;

  puaFileSelectorSort( files, dflag, num_files ) ;

  list_box -> newList ( files ) ;
}

