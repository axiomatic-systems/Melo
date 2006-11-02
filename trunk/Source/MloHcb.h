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
