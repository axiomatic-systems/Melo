/*
Single Channel Element & LFE channel Element

Tow of the Syntactic Elements contained in the raw data blocks.

Ref: 4.4.2.1, Table 4.4
*/



#ifndef _MLO_ELEMENT_SCE_LFE_H_
#define _MLO_ELEMENT_SCE_LFE_H_



/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/



#include "MloSamplingFreq.h"
#include "MloBitStream.h"
#include "MloIndivChnPool.h"
#include "MloIndivChnStream.h"
#include "MloTypes.h"



/*----------------------------------------------------------------------
|       Types
+---------------------------------------------------------------------*/



/* Ref: 4.4.2.1, Table 4.4 */
typedef struct MLO_ElementSceLfe
{
   int            element_instance_tag;
   MLO_IndivChnStream *
                  ics_ptr;  /* Allocated by the channel pool */
}  MLO_ElementSceLfe;



/*----------------------------------------------------------------------
|       Function prototypes
+---------------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



MLO_Result  MLO_ElementSceLfe_Decode (MLO_ElementSceLfe *sce_ptr, MLO_BitStream *bit_ptr, MLO_IndivChnPool *chn_pool_ptr, MLO_SamplingFreq_Index fs_index);



#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* _MLO_ELEMENT_SCE_LFE_H_ */
