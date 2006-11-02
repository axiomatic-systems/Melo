/*
Fill Element

One of the Syntactic Elements contained in the raw data blocks.

Ref:
4.4.1.1, Table 4.11
4.4.2.7, Table 4.51
4.5.2.9
*/

#ifndef _MLO_ELEMENT_FIL_H_
#define _MLO_ELEMENT_FIL_H_



/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/



#include "MloBitStream.h"
#include "MloTypes.h"



/*----------------------------------------------------------------------
|       Constants
+---------------------------------------------------------------------*/



#define  MLO_ERROR_SBR_IN_LC           (MLO_ERROR_BASE_ELEMENT_FIL-0)
#define  MLO_ERROR_WRONG_FILL_NIBBLE   (MLO_ERROR_BASE_ELEMENT_FIL-1)
#define  MLO_ERROR_WRONG_FILL_BYTE     (MLO_ERROR_BASE_ELEMENT_FIL-2)



/*----------------------------------------------------------------------
|       Types
+---------------------------------------------------------------------*/



enum {   MLO_ELEMENT_FIL_DYN_RANGE_INFO_MAX_NBR_BANDS = 17  };

/* 4.4.2.7, Table 4.52 */
/*** Ca faudra peut-etre le deplacer dans un module
   a part, particulierement a cause de l'init ***/
typedef struct MLO_ElementFil_DynamicRangeInfo
{
   int            num_bands;
   int            pce_instance_tag;
   int            tag_reserved_bits;
   MLO_Boolean    excluded_chns_present_flag;
   MLO_UInt32     excluded_mask; /* 1 bit per channel. Here, max number of channels = 32 */
   int            band_top_arr [MLO_ELEMENT_FIL_DYN_RANGE_INFO_MAX_NBR_BANDS];
   int            prog_ref_level;
   int            dyn_rng_sgn [MLO_ELEMENT_FIL_DYN_RANGE_INFO_MAX_NBR_BANDS]; /* -1 or +1 */
   int            dyn_rng_ctl [MLO_ELEMENT_FIL_DYN_RANGE_INFO_MAX_NBR_BANDS];
}  MLO_ElementFil_DynamicRangeInfo;



typedef struct MLO_ElementFil
{
   MLO_ElementFil_DynamicRangeInfo
                  dynamic_range_info;
}  MLO_ElementFil;


/*----------------------------------------------------------------------
|       Function prototypes
+---------------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



MLO_Result  MLO_ElementFil_Decode (MLO_ElementFil *fil_ptr, MLO_BitStream *bit_ptr);



#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* _MLO_ELEMENT_FIL_H_ */
