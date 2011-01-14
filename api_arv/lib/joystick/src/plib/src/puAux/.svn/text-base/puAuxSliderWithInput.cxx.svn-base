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

     $Id: puAuxSliderWithInput.cxx,v 1.1 2004/02/28 00:54:03 sjbaker Exp $
*/

#include "puAux.h"

UL_RTTI_DEF1(puaSliderWithInput,puGroup)

void puaSliderWithInput::handle_slider ( puObject *obj )
{
  puaSliderWithInput *sl = (puaSliderWithInput *)obj->getUserData () ;
  sl->setValue ( obj->getFloatValue () ) ;
  sl->__setInputBox ( obj->getFloatValue () ) ;

  sl->invokeCallback () ;
}

void puaSliderWithInput::handle_input ( puObject *obj  )
{
  puaSliderWithInput *sl = (puaSliderWithInput *)obj->getUserData () ;
  sl->setValue ( obj->getFloatValue () ) ;
  sl->invokeCallback () ;
}

puaSliderWithInput::puaSliderWithInput ( int minx, int miny, int maxx, int maxy, int above ) :
                               puGroup ( minx, miny )
{
  type |= PUCLASS_SLIDERWITHINPUT ;
  slider = new puSlider ( (maxx - minx)/2-10, above ? 0 : 20, maxy-miny-20, TRUE ) ;
  input_box = new puInput ( 0, above ? maxy - miny - 20 : 0, maxx-minx, above ? maxy - miny : 20 ) ;
  input_box->setValue ( 0 ) ;
  slider->setUserData ( this ) ;
  slider->setCallback ( handle_slider ) ;
  input_box->setUserData ( this ) ;
  input_box->setCallback ( handle_input ) ;
  close () ;

  input_position = above ;
}


void puaSliderWithInput::setSize ( int w, int h )
{
  slider->setSize ( 20, h-40 ) ;
  slider->setPosition ( w/2-10, input_position ? 0 : 20 ) ;

  input_box->setSize ( w, 20 ) ;
  input_box->setPosition ( 0, input_position ? h-20 : 0 ) ;
}


void puaSliderWithInput::draw ( int dx, int dy )
{
  if ( !visible || ( window != puGetWindow () ) ) return ;

  draw_label ( dx, dy ) ;

  puGroup::draw ( dx, dy ) ;
}


int puaSliderWithInput::checkKey ( int key, int updown )
{
  if ( ! isVisible () || ! isActive () || ( window != puGetWindow () ) )
    return FALSE ;

  return ( input_box->checkKey ( key, updown ) ) ;
}

