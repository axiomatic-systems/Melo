#ifndef _MLO_SAMPLING_FREQ_H_
#define _MLO_SAMPLING_FREQ_H_

/*----------------------------------------------------------------------
|    include
+---------------------------------------------------------------------*/
#include "MloConfig.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* 1.6.3.4, Table 1.16 */
typedef enum MLO_SamplingFreq_Index
{
   MLO_SAMPLING_FREQ_INDEX_INVALID = -1,

   MLO_SAMPLING_FREQ_96000,
   MLO_SAMPLING_FREQ_88200,
   MLO_SAMPLING_FREQ_64000,
   MLO_SAMPLING_FREQ_48000,
   MLO_SAMPLING_FREQ_44100,
   MLO_SAMPLING_FREQ_32000,
   MLO_SAMPLING_FREQ_24000,
   MLO_SAMPLING_FREQ_22050,
   MLO_SAMPLING_FREQ_16000,
   MLO_SAMPLING_FREQ_12000,
   MLO_SAMPLING_FREQ_11025,
   MLO_SAMPLING_FREQ_8000,
   MLO_SAMPLING_FREQ_7350,

   MLO_SAMPLING_FREQ_INDEX_NBR_SUPPORTED = 12,   /* 7350 Hz is not supported by some parts of the specs */
   MLO_SAMPLING_FREQ_INDEX_NBR_VALID = 13,
   MLO_SAMPLING_FREQ_INDEX_RESERVED = MLO_SAMPLING_FREQ_INDEX_NBR_VALID,
   MLO_SAMPLING_FREQ_INDEX_ESCAPE = 15,

   MLO_SAMPLING_FREQ_INDEX_NBR_ELT
}  MLO_SamplingFreq_Index;



extern const MLO_UInt32 
MLO_SamplingFreq_table [MLO_SAMPLING_FREQ_INDEX_NBR_ELT];


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _MLO_SAMPLING_FREQ_H_ */
