/*
Program Config Element

One of the Syntactic Elements contained in the raw data blocks.

Ref:
4.4.1.1, Table 4.2
4.5.2.1
*/

#ifndef _MLO_ELEMENT_PCE_H_
#define _MLO_ELEMENT_PCE_H_



/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/



#include "MloSamplingFreq.h"
#include "MloBitStream.h"
#include "MloFrame.h"
#include "MloTypes.h"



/*----------------------------------------------------------------------
|       Types
+---------------------------------------------------------------------*/



typedef enum MLO_ElementPce_MaxNbr
{
   MLO_ELEMENT_PCE_MAX_NBR_FRONT_CHN_ELT  = 1 << 4,
   MLO_ELEMENT_PCE_MAX_NBR_SIDE_CHN_ELT   = 1 << 4,
   MLO_ELEMENT_PCE_MAX_NBR_BACK_CHN_ELT   = 1 << 4,
   MLO_ELEMENT_PCE_MAX_NBR_LFE_CHN_ELT    = 1 << 2,
   MLO_ELEMENT_PCE_MAX_NBR_ASSOC_DATA_ELT = 1 << 3,
   MLO_ELEMENT_PCE_MAX_NBR_VALID_CC_ELT   = 1 << 4
}  MLO_ElementPce_MaxNbr;



typedef struct MLO_ElementPce_TaggedElt
{
   MLO_Boolean    is_cpe_flag;
   int            tag;
}  MLO_ElementPce_TaggedElt;



typedef struct MLO_ElementPce_Cce
{
   MLO_Boolean    is_ind_sw_flag;
   int            tag;
}  MLO_ElementPce_Cce;



/* 4.4.1.1, Table 4.2 */
typedef struct MLO_ElementPce
{
   int            element_instance_tag;
   MLO_AacProfile object_type;
   MLO_SamplingFreq_Index
                  sampling_frequency_index;
   int            num_front_channel_elements;
   int            num_side_channel_elements;
   int            num_back_channel_elements;
   int            num_lfe_channel_elements;
   int            num_assoc_data_elements;
   int            num_valid_cc_elements;
   MLO_Boolean    mono_mixdown_present;
   int            mono_mixdown_element_number;
   MLO_Boolean    stereo_mixdown_present;
   int            stereo_mixdown_element_number;
   MLO_Boolean    matrix_mixdown_idx_present;
   int            matrix_mixdown_idx ;
   MLO_Boolean    pseudo_surround_enable;
   MLO_ElementPce_TaggedElt
                  front_element_arr [MLO_ELEMENT_PCE_MAX_NBR_FRONT_CHN_ELT];
   MLO_ElementPce_TaggedElt
                  side_element_arr [MLO_ELEMENT_PCE_MAX_NBR_SIDE_CHN_ELT];
   MLO_ElementPce_TaggedElt
                  back_element_arr [MLO_ELEMENT_PCE_MAX_NBR_BACK_CHN_ELT];
   int            lfe_element_tag_arr [MLO_ELEMENT_PCE_MAX_NBR_LFE_CHN_ELT];
   int            assoc_data_element_tag_arr [MLO_ELEMENT_PCE_MAX_NBR_ASSOC_DATA_ELT];
   MLO_ElementPce_Cce
                  cc_element_arr [MLO_ELEMENT_PCE_MAX_NBR_VALID_CC_ELT];
}  MLO_ElementPce;



/*----------------------------------------------------------------------
|       Function prototypes
+---------------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



MLO_Result  MLO_ElementPce_Decode (MLO_ElementPce *pce_ptr, MLO_BitStream *bit_ptr);



#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* _MLO_ELEMENT_PCE_H_ */
