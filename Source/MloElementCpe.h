/*
Channel Pair Element

One of the Syntactic Elements contained in the raw data blocks.

Ref:
4.4.2.1, Table 4.5
*/



#ifndef _MLO_ELEMENT_CPE_H_
#define _MLO_ELEMENT_CPE_H_



/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/



#include "MloSamplingFreq.h"
#include "MloBitStream.h"
#include "MloDefs.h"
#include "MloIndivChnPool.h"
#include "MloIndivChnStream.h"
#include "MloTypes.h"



/*----------------------------------------------------------------------
|       Function prototypes
+---------------------------------------------------------------------*/



/* Ref: 4.6.8.1.2 */
typedef enum MLO_ElementCpe_MsMaskType
{
   MLO_ELEMENT_CPE_MS_MASK_TYPE_ALL_0 = 0,
   MLO_ELEMENT_CPE_MS_MASK_TYPE_USED,
   MLO_ELEMENT_CPE_MS_MASK_TYPE_ALL_1,
   MLO_ELEMENT_CPE_MS_MASK_TYPE_RESERVED,

   MLO_ELEMENT_CPE_MS_MASK_TYPE_NBR_ELT
}  MLO_ElementCpe_MsMaskType;

typedef struct MLO_ElementCpe
{
   int            element_instance_tag;
   MLO_Boolean    common_window_flag;
   MLO_ElementCpe_MsMaskType
                  ms_mask_present;  /* Set to ALL_0 if common_window_flag is not set */
   MLO_UInt8      ms_used [MLO_DEFS_MAX_NBR_WIN_GRP] [MLO_DEFS_MAX_NUM_SWB + 1];   /* [g] [sfb]. Boolean */
   MLO_IndivChnStream *
                  ics_ptr_arr [2];  /* Allocated on the channel pool */
}  MLO_ElementCpe;



/*----------------------------------------------------------------------
|       Function prototypes
+---------------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



MLO_Result  MLO_ElementCpe_Decode (MLO_ElementCpe *cpe_ptr, MLO_BitStream *bit_ptr, MLO_IndivChnPool *chn_pool_ptr, MLO_SamplingFreq_Index fs_index);



#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* _MLO_ELEMENT_CPE_H_ */
