/*
Data Stream Element

One of the Syntactic Elements contained in the raw data blocks.
A data element contains any additional data, e.g. auxiliary information,
that is not part of the audio information itself.

Ref:
4.4.1.1, Table 4.10
4.5.2.1.4
*/

#ifndef _MLO_ELEMENT_DSE_H_
#define _MLO_ELEMENT_DSE_H_



/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/



#include "MloBitStream.h"
#include "MloTypes.h"



/*----------------------------------------------------------------------
|       Function prototypes
+---------------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



MLO_Result  MLO_ElementDse_Decode (MLO_BitStream *bit_ptr);



#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* _MLO_ELEMENT_DSE_H_ */
