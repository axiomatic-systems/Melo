/*
Huffman decoding

Ref:
*/

#ifndef _MLO_HUFFMAN_H_
#define _MLO_HUFFMAN_H_



/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/



#include "MloBitStream.h"
#include "MloHcb.h"
#include "MloTypes.h"



/*----------------------------------------------------------------------
|       Constants
+---------------------------------------------------------------------*/



#define  MLO_ERROR_UNEXPECTED_CB_TYPE  (MLO_ERROR_BASE_HUFFMAN-0)



/*----------------------------------------------------------------------
|       Function prototypes
+---------------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



MLO_Result  MLO_Huffman_decode_spectral_data (MLO_Int16 data_ptr [], MLO_BitStream *bit_ptr, MLO_Hcb hcb);



#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* _MLO_HUFFMAN_H_ */
