/*
Huffman CodeBooks for data pairs
*/

#ifndef _MLO_HCP_PAIR_H_
#define _MLO_HCP_PAIR_H_



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



void  MLO_HcbPair_decode_binary (MLO_Int16 data_ptr [2], MLO_BitStream *bit_ptr, MLO_Hcb hcb);
void  MLO_HcbPair_decode_2steps (MLO_Int16 data_ptr [2], MLO_BitStream *bit_ptr, MLO_Hcb hcb);



#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* _MLO_HCP_PAIR_H_ */
