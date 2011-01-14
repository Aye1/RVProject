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

     $Id: puAuxSpinBox.cxx,v 1.1 2004/02/28 00:54:03 sjbaker Exp $
*/


#include "puAux.h"

UL_RTTI_DEF2(puaSpinBox,puRange,puGroup)


puaSpinBox::puaSpinBox ( int minx, int miny, int maxx, int maxy, int arrow_pos ) :
               puRange ( 1.0f, 10.0f, 1.0f ), puGroup ( minx, miny )
{
  extern void puaSpinBox_handle_input ( puObject* ob ) ;
  extern void puaSpinBox_handle_input_active ( puObject* ob ) ;
  extern void puaSpinBox_handle_arrow ( puObject* ob ) ;
  type |= PUCLASS_SPINBOX ;
  arrow_position = arrow_pos ;
  int arrow_size = ( maxy - miny ) / 2 ;
  if ( arrow_pos == 1 )
    input_box = new puInput ( 0, 0, maxx - minx - arrow_size, maxy - miny ) ;
  else
    input_box = new puInput ( arrow_size, 0, maxx - minx, maxy - miny ) ;

  input_box->setCallback ( puaSpinBox_handle_input ) ;
  input_box->setActiveCallback ( puaSpinBox_handle_input_active ) ;
  input_box->setDownCallback ( puaSpinBox_handle_input ) ;
  input_box->setUserData ( this ) ;

  up_arrow = new puArrowButton ( arrow_position?maxx-minx-arrow_size:0, arrow_size,
                                 arrow_position?maxx-minx:arrow_size, maxy-miny, PUARROW_UP ) ;
  up_arrow->setCallback ( puaSpinBox_handle_arrow ) ;
  up_arrow->setUserData ( this ) ;

  down_arrow = new puArrowButton ( arrow_position?maxx-minx-arrow_size:0, 0,
                                   arrow_position?maxx-minx:arrow_size, arrow_size, PUARROW_DOWN ) ;
  down_arrow->setCallback ( puaSpinBox_handle_arrow ) ;
  down_arrow->setUserData ( this ) ;

  close () ;
}

void puaSpinBox_handle_arrow ( puObject *ob ) 
{
  puaSpinBox *master = (puaSpinBox *)(ob->getUserData ()) ;
  float val = master->getFloatValue () ;
  if ( ((puArrowButton *)ob)->getArrowType () == PUARROW_UP )
    val += master->getStepSize () ;
  else
    val -= master->getStepSize () ;

  if ( val > master->getMaxValue () ) val = master->getMaxValue () ;
  if ( val < master->getMinValue () ) val = master->getMinValue () ;
  master->setValue ( val ) ;
  master->invokeCallback () ;
}

void puaSpinBox_handle_input ( puObject *ob ) 
{
  puaSpinBox *master = (puaSpinBox *)(ob->getUserData ()) ;
  float val = ob->getFloatValue () ;
  if ( val > master->getMaxValue () ) val = master->getMaxValue () ;
  if ( val < master->getMinValue () ) val = master->getMinValue () ;
  master->setValue ( val ) ;
  master->invokeCallback () ;
}

void puaSpinBox_handle_input_active ( puObject *ob ) 
{
  puaSpinBox *master = (puaSpinBox *)(ob->getUserData ()) ;
  float val = ob->getFloatValue () ;
  if ( val > master->getMaxValue () ) val = master->getMaxValue () ;
  if ( val < master->getMinValue () ) val = master->getMinValue () ;
  master->setValue ( val ) ;
  master->invokeActiveCallback () ;
}

