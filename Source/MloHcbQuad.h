/*
Huffman CodeBooks for data quads
*/

#ifndef _MLO_HCB_QUAD_H_
#define _MLO_HCB_QUAD_H_



/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/



#include "MloBitStream.h"
#include "MloHcb.h"
#include "MloTypes.h"



/*----------------------------------------------------------------------
|       Function prototypes
+---------------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



void  MLO_HcbQuad_decode_binary (MLO_Int16 data_ptr [4], MLO_BitStream *bit_ptr, MLO_Hcb hcb);
void  MLO_HcbQuad_decode_2steps (MLO_Int16 data_ptr [4], MLO_BitStream *bit_ptr, MLO_Hcb hcb);



#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* _MLO_HCB_QUAD_H_ */
