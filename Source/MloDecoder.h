/*****************************************************************
|
|      File: MloDecoder.h
|
|      Melo - Decoder API
|
|      (c) 2004 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file
 * Melo Decoder API
 */

#ifndef _MLO_DECODER_H_
#define _MLO_DECODER_H_

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "MloFrame.h"
#include "MloBitStream.h"
#include "MloSampleBuffer.h"
#include "MloSamplingFreq.h"

/*----------------------------------------------------------------------
|       types
+---------------------------------------------------------------------*/
typedef struct MLO_Decoder MLO_Decoder;

typedef struct {
    MLO_Flags    flags;
    MLO_Cardinal frame_count;
} MLO_DecoderStatus;

typedef enum {
    MLO_OBJECT_TYPE_AAC_MAIN     = 1,
    MLO_OBJECT_TYPE_AAC_LC       = 2,
    MLO_OBJECT_TYPE_AAC_SSR      = 3,
    MLO_OBJECT_TYPE_AAC_LTP      = 4,
    MLO_OBJECT_TYPE_SBR          = 5,
    MLO_OBJECT_TYPE_AAC_SCALABLE = 6
} MLO_ObjectTypeIdentifier;

typedef struct {
    MLO_ObjectTypeIdentifier object_type;
    MLO_SamplingFreq_Index   sampling_frequency_index;
} MLO_DecoderConfig;

/*----------------------------------------------------------------------
|       prototypes
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

MLO_Result MLO_DecoderConfig_Parse(const unsigned char* encoded, 
                                   MLO_Size             encoded_size,
                                   MLO_DecoderConfig*   config);

MLO_Result MLO_Decoder_Create(const MLO_DecoderConfig* config,
                              MLO_Decoder**            decoder);
MLO_Result MLO_Decoder_Destroy(MLO_Decoder* decoder);
MLO_Result MLO_Decoder_Reset(MLO_Decoder* decoder);
MLO_Result MLO_Decoder_DecodeFrame(MLO_Decoder*       decoder,
                                   const MLO_Byte*    frame,
                                   MLO_Size           frame_size,
                                   MLO_SampleBuffer*  sample);
MLO_Result MLO_Decoder_GetStatus(MLO_Decoder*        decoder, 
                                 MLO_DecoderStatus** status);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MLO_DECODER_H_ */
