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

     $Id: puSpinBox.cxx,v 1.7 2003/02/07 17:01:03 nhv Exp $
*/


#include "puLocal.h"

UL_RTTI_DEF2(puSpinBox,puRange,puGroup)


puSpinBox::puSpinBox ( int minx, int miny, int maxx, int maxy, int arrow_pos ) :
                       puRange ( 1.0f, 10.0f, 1.0f ), puGroup ( minx, miny )
{
  extern void puSpinBox_handle_input ( puObject* ob ) ;
  extern void puSpinBox_handle_input_active ( puObject* ob ) ;
  extern void puSpinBox_handle_arrow ( puObject* ob ) ;
  type |= PUCLASS_SPINBOX ;
  arrow_position = arrow_pos ;
  int arrow_size = ( maxy - miny ) / 2 ;
  if ( arrow_pos == 1 )
    input_box = new puInput ( 0, 0, maxx - minx - arrow_size, maxy - miny ) ;
  else
    input_box = new puInput ( arrow_size, 0, maxx - minx, maxy - miny ) ;

  input_box->setCallback ( puSpinBox_handle_input ) ;
  input_box->setActiveCallback ( puSpinBox_handle_input_active ) ;
  input_box->setDownCallback ( puSpinBox_handle_input ) ;
  input_box->setUserData ( this ) ;

  up_arrow = new puArrowButton ( arrow_position?maxx-minx-arrow_size:0, arrow_size,
                                 arrow_position?maxx-minx:arrow_size, maxy-miny, PUARROW_UP ) ;
  up_arrow->setCallback ( puSpinBox_handle_arrow ) ;
  up_arrow->setUserData ( this ) ;

  down_arrow = new puArrowButton ( arrow_position?maxx-minx-arrow_size:0, 0,
                                   arrow_position?maxx-minx:arrow_size, arrow_size, PUARROW_DOWN ) ;
  down_arrow->setCallback ( puSpinBox_handle_arrow ) ;
  down_arrow->setUserData ( this ) ;

  close () ;
}

void puSpinBox_handle_arrow ( puObject *ob ) 
{
  puSpinBox *master = (puSpinBox *)(ob->getUserData ()) ;
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

void puSpinBox_handle_input ( puObject *ob ) 
{
  puSpinBox *master = (puSpinBox *)(ob->getUserData ()) ;
  float val = ob->getFloatValue () ;
  if ( val > master->getMaxValue () ) val = master->getMaxValue () ;
  if ( val < master->getMinValue () ) val = master->getMinValue () ;
  master->setValue ( val ) ;
  master->invokeCallback () ;
}

void puSpinBox_handle_input_active ( puObject *ob ) 
{
  puSpinBox *master = (puSpinBox *)(ob->getUserData ()) ;
  float val = ob->getFloatValue () ;
  if ( val > master->getMaxValue () ) val = master->getMaxValue () ;
  if ( val < master->getMinValue () ) val = master->getMinValue () ;
  master->setValue ( val ) ;
  master->invokeActiveCallback () ;
}

