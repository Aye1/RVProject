/*
     PUI Auxiliary Widget Library
     Derived from PLIB, the Portable Game Library by Steve Baker.
     Copyright (C) 1998,2002,2004  Steve Baker

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

     $Id: puAuxBiSliderWithEnds.cxx,v 1.1 2004/02/28 00:54:03 sjbaker Exp $
*/

#include "puAux.h"

UL_RTTI_DEF1(puaBiSliderWithEnds,puGroup)

void puaBiSliderWithEnds::handle_slider ( puObject *obj )
{
  puaBiSliderWithEnds *bisl = (puaBiSliderWithEnds *)obj->getUserData () ;
  bisl->__setMax ( ((puBiSlider *)obj)->getCurrentMax () ) ;
  bisl->__setMin ( ((puBiSlider *)obj)->getCurrentMin () ) ;

  bisl->invokeCallback () ;
}

void puaBiSliderWithEnds::handle_max ( puObject *obj  )
{
  puaBiSliderWithEnds *bisl = (puaBiSliderWithEnds *)obj->getUserData () ;
  bisl->setCurrentMax ( obj->getFloatValue () ) ;
  bisl->invokeCallback () ;
}

void puaBiSliderWithEnds::handle_min ( puObject *obj )
{
  puaBiSliderWithEnds *bisl = (puaBiSliderWithEnds *)obj->getUserData () ;
  bisl->setCurrentMin ( obj->getFloatValue () ) ;
  bisl->invokeCallback () ;
}

puaBiSliderWithEnds::puaBiSliderWithEnds ( int minx, int miny, int maxx, int maxy ) :
                                 puGroup ( minx, miny )
{
  type |= PUCLASS_BISLIDERWITHENDS ;
  slider = new puaBiSlider ( (maxx - minx)/2-10, 20, maxy-miny-40, TRUE ) ;
  min_box = new puInput ( 0, 0, maxx-minx, 20 ) ;
  max_box = new puInput ( 0, maxy-miny-20, maxx-minx, maxy-miny ) ;
  min_box->setValue ( 0 ) ;
  max_box->setValue ( 1 ) ;
  slider->setUserData ( this ) ;
  slider->setCallback ( handle_slider ) ;
  max_box->setUserData ( this ) ;
  max_box->setCallback ( handle_max ) ;
  min_box->setUserData ( this ) ;
  min_box->setCallback ( handle_min ) ;
  close () ;
}


void puaBiSliderWithEnds::setSize ( int w, int h )
{
  slider->setSize ( 20, h-40 ) ;
  slider->setPosition ( w/2-10, 20 ) ;

  min_box->setSize ( w, 20 ) ;

  max_box->setSize ( w, 20 ) ;
  max_box->setPosition ( 0, h-20 ) ;
}


void puaBiSliderWithEnds::draw ( int dx, int dy )
{
  if ( !visible || ( window != puGetWindow () ) ) return ;

  draw_label ( dx, dy ) ;

  puGroup::draw ( dx, dy ) ;
}


int puaBiSliderWithEnds::checkKey ( int key, int updown )
{
  if ( ! isVisible () || ! isActive () || ( window != puGetWindow () ) )
    return FALSE ;

  return ( max_box->checkKey ( key, updown ) + min_box->checkKey ( key, updown ) ) ;
}

