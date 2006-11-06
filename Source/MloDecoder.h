/*****************************************************************
|
|    Melo - Decoder API
|
|    Copyright 2004-2006 Axiomatic Systems LLC
|
|    This file is part of Melo (Melo AAC Decoder).
|
|    Unless you have obtained Melo under a difference license,
|    this version of Melo is Melo|GPL.
|    Melo|GPL is free software; you can redistribute it and/or modify
|    it under the terms of the GNU General Public License as published by
|    the Free Software Foundation; either version 2, or (at your option)
|    any later version.
|
|    Melo|GPL is distributed in the hope that it will be useful,
|    but WITHOUT ANY WARRANTY; without even the implied warranty of
|    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|    GNU General Public License for more details.
|
|    You should have received a copy of the GNU General Public License
|    along with Melo|GPL; see the file COPYING.  If not, write to the
|    Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
|    02111-1307, USA.
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
    MLO_OBJECT_TYPE_AAC_MAIN        = 1,
    MLO_OBJECT_TYPE_AAC_LC          = 2,
    MLO_OBJECT_TYPE_AAC_SSR         = 3,
    MLO_OBJECT_TYPE_AAC_LTP         = 4,
    MLO_OBJECT_TYPE_SBR             = 5,
    MLO_OBJECT_TYPE_AAC_SCALABLE    = 6,
    MLO_OBJECT_TYPE_TWINVQ          = 7,
    MLO_OBJECT_TYPE_ER_AAC_LC       = 17,
    MLO_OBJECT_TYPE_ER_AAC_LTP      = 19,
    MLO_OBJECT_TYPE_ER_AAC_SCALABLE = 20,
    MLO_OBJECT_TYPE_ER_TWINVQ       = 21,
    MLO_OBJECT_TYPE_ER_BSAC         = 22,
    MLO_OBJECT_TYPE_ER_AAC_LD       = 23,
    MLO_OBJECT_TYPE_LAYER_1         = 32,
    MLO_OBJECT_TYPE_LAYER_2         = 33,
    MLO_OBJECT_TYPE_LAYER_3         = 34
} MLO_ObjectTypeIdentifier;

typedef enum {
    MLO_CHANNEL_CONFIG_NONE   = 0,
    MLO_CHANNEL_CONFIG_MONO   = 1,
    MLO_CHANNEL_CONFIG_STEREO = 2,
    MLO_CHANNEL_CONFIG_STEREO_PLUS_CENTER = 3,
    MLO_CHANNEL_CONFIG_STEREO_PLUS_CENTER_PLUS_REAR_MONO = 4,
    MLO_CHANNEL_CONFIG_FIVE = 5,
    MLO_CHANNEL_CONFIG_FIVE_PLUS_ONE = 6,
    MLO_CHANNEL_CONFIG_SEVEN_PLUS_ONE = 7,
    MLO_CHANNEL_CONFIG_UNSUPPORTED
} MLO_ChannelConfiguration;

typedef struct {
    MLO_ObjectTypeIdentifier object_type;
    MLO_SamplingFreq_Index   sampling_frequency_index;
    MLO_ChannelConfiguration channel_configuration;
    MLO_Boolean              frame_length_flag;
    MLO_Boolean              depends_on_core_coder;
    MLO_Boolean              core_coder_delay;
    struct {
        MLO_Boolean              sbr_present;
        MLO_ObjectTypeIdentifier object_type;
        unsigned int             sampling_frequency_index;
    } extension;
} MLO_DecoderConfig;

/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
#define MLO_DECODER_MAX_FRAME_SIZE 8192

#define MLO_ERROR_DECODER_UNSUPPORTED_CONFIG            (MLO_ERROR_BASE_DECODER-0)
#define MLO_ERROR_DECODER_INVALID_CHANNEL_CONFIGURATION (MLO_ERROR_BASE_DECODER-1)
#define MLO_ERROR_DECODER_UNSUPPORTED_FORMAT            (MLO_ERROR_BASE_DECODER-2)
#define MLO_ERROR_DECODER_INVALID_DATA                  (MLO_ERROR_BASE_DECODER-3)

/*----------------------------------------------------------------------
|   prototypes
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
