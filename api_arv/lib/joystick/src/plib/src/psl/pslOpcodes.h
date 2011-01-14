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

     $Id: pslOpcodes.h,v 1.14 2002/09/22 18:25:16 sjbaker Exp $
*/


/* Code Opcodes */

#define OPCODE_BAD                   0x00
#define OPCODE_PUSH_INT_CONSTANT     0x01
#define OPCODE_PUSH_FLOAT_CONSTANT   0x02
#define OPCODE_PUSH_STRING_CONSTANT  0x03
#define OPCODE_CALL                  0x04
#define OPCODE_SUB                   0x05
#define OPCODE_ADD                   0x06
#define OPCODE_DIV                   0x07
#define OPCODE_MULT                  0x08
#define OPCODE_MOD                   0x09
#define OPCODE_NEG                   0x0A
#define OPCODE_NOT                   0x0B
#define OPCODE_TWIDDLE               0x0C
#define OPCODE_OROR                  0x0D
#define OPCODE_ANDAND                0x0E
#define OPCODE_SHIFTLEFT             0x0F

#define OPCODE_SHIFTRIGHT            0x10
#define OPCODE_OR                    0x11
#define OPCODE_AND                   0x12
#define OPCODE_XOR                   0x13
#define OPCODE_LESS                  0x14
#define OPCODE_LESSEQUAL             0x15
#define OPCODE_GREATER               0x16
#define OPCODE_GREATEREQUAL          0x17
#define OPCODE_NOTEQUAL              0x18
#define OPCODE_EQUAL                 0x19
#define OPCODE_JUMP_FALSE            0x1A
#define OPCODE_JUMP_TRUE             0x1B
#define OPCODE_JUMP                  0x1C
#define OPCODE_POP                   0x1D
#define OPCODE_HALT                  0x1E
#define OPCODE_CALLEXT               0x1F

#define OPCODE_PAUSE                 0x20
#define OPCODE_RETURN                0x21
#define OPCODE_PUSH_VARIABLE         0x22
#define OPCODE_POP_VARIABLE          0x23
#define OPCODE_SET_INT_VARIABLE      0x24
#define OPCODE_SET_FLOAT_VARIABLE    0x25
#define OPCODE_SET_STRING_VARIABLE   0x26
#define OPCODE_STACK_DUPLICATE       0x27
#define OPCODE_GET_PARAMETER         0x28

#define OPCODE_PEEK_JUMP_FALSE       0x2B
#define OPCODE_PEEK_JUMP_TRUE        0x2C
#define OPCODE_LINE_NUMBER           0x2D



#define OPCODE_POP_ADD_VARIABLE      0x30
#define OPCODE_POP_SUB_VARIABLE      0x31
#define OPCODE_POP_MUL_VARIABLE      0x32
#define OPCODE_POP_MOD_VARIABLE      0x33
#define OPCODE_POP_DIV_VARIABLE      0x34
#define OPCODE_POP_AND_VARIABLE      0x35
#define OPCODE_POP_OR_VARIABLE       0x36
#define OPCODE_POP_XOR_VARIABLE      0x37
#define OPCODE_POP_SHL_VARIABLE      0x38
#define OPCODE_POP_SHR_VARIABLE      0x39
#define OPCODE_EXCHANGE              0x3A
#define OPCODE_SET_INT_ARRAY         0x3B
#define OPCODE_SET_FLOAT_ARRAY       0x3C
#define OPCODE_SET_STRING_ARRAY      0x3D



#define OPCODE_FETCH                 0x40
#define OPCODE_INCREMENT_FETCH       0x41
#define OPCODE_DECREMENT_FETCH       0x42
#define OPCODE_INCREMENT_LVALUE      0x43
#define OPCODE_DECREMENT_LVALUE      0x44


