/*****************************************************************
|
|    Copyright 2004-2006 Axiomatic Systems LLC
|
|    This file is part of Melo (Melo AAC Decoder).
|
|    Unless you have obtained Melo under a difference license,
|    this version of Melo is Melo|GPL.
|    Melo|GPL is free software; you can redistribute it and/or modify
|    it under the terms of the GNU General Public License as published by
|    the Free Software Foundation; either version 2, or (at your option)
|    any later version.
|
|    Melo|GPL is distributed in the hope that it will be useful,
|    but WITHOUT ANY WARRANTY; without even the implied warranty of
|    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|    GNU General Public License for more details.
|
|    You should have received a copy of the GNU General Public License
|    along with Melo|GPL; see the file COPYING.  If not, write to the
|    Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
|    02111-1307, USA.
|
 ****************************************************************/

/*
Huffman CodeBook types

Ref: 4.6.3.2
*/

#ifndef _MLO_ELEMENT_PCE_H_
#define _MLO_ELEMENT_PCE_H_



typedef enum MLO_Hcb
{
   MLO_HCB_ZERO_HCB       = 0,
   MLO_HCB_FIRST_PAIR_HCB = 5,
   MLO_HCB_ESC_HCB        = 11,
   MLO_HCB_RESERVED       = 12,
   MLO_HCB_NOISE_HCB      = 13,
   MLO_HCB_INTENSITY_HCB2 = 14,
   MLO_HCB_INTENSITY_HCB  = 15,
   MLO_HCB_ESC_FLAG       = 16,

   MLO_HCB_NBR_ELT
}  MLO_Hcb;



#endif /* _MLO_ELEMENT_PCE_H_ */
