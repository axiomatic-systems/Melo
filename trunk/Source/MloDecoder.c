/*****************************************************************
|
|      File: MloDecoder.c
|
|      Melo - Decoder
|
|      (c) 2004 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "MloConfig.h"
#include "MloDebug.h"
#include "MloDefs.h"
#include "MloBitStream.h"
#include "MloDecoder.h"
#include "MloElementDse.h"
#include "MloElementFil.h"
#include "MloElementPce.h"
#include "MloFilterBank.h"
#include "MloFrame.h"
#include "MloSyntacticElements.h"
#include "MloTypes.h"
#include "MloUtils.h"

/*----------------------------------------------------------------------
|   types
+---------------------------------------------------------------------*/
struct MLO_Decoder {
    MLO_DecoderConfig      config;
    MLO_DecoderStatus      status;
    MLO_SyntacticElements  syntactic_elements;
    MLO_FilterBank         filter_bank;
    MLO_BitStream          bitstream;
};

/*----------------------------------------------------------------------
|   prototypes
+---------------------------------------------------------------------*/
MLO_Result  
MLO_Decoder_DecodeFrameContent (
   MLO_Decoder *        decoder_ptr, 
   MLO_BitStream *      bits_ptr,
   MLO_SampleBuffer *   buffer_ptr
);

/*----------------------------------------------------------------------
|   GetAudioObjectType
+---------------------------------------------------------------------*/
static MLO_Result
GetAudioObjectType(MLO_BitStream* bits, MLO_ObjectTypeIdentifier* audio_object_type)
{
    if (MLO_BitStream_GetBitsLeft(bits) < 5) return MLO_ERROR_DECODER_INVALID_DATA;
    *audio_object_type = MLO_BitStream_ReadBits(bits, 5);
	if (*audio_object_type == 31) {
        if (MLO_BitStream_GetBitsLeft(bits) < 6) return MLO_ERROR_DECODER_INVALID_DATA;
		*audio_object_type = 32 + MLO_BitStream_ReadBits(bits, 6);
	}
	return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|   GetGASpecificInfo
+---------------------------------------------------------------------*/
static MLO_Result
GetGASpecificInfo(MLO_BitStream* bits, MLO_DecoderConfig* config)
{
    if (MLO_BitStream_GetBitsLeft(bits) < 2) return MLO_ERROR_DECODER_INVALID_DATA;
	config->frame_length_flag = MLO_BitStream_ReadBit(bits);
	config->depends_on_core_coder = MLO_BitStream_ReadBit(bits);
	if (config->depends_on_core_coder) {		
        if (MLO_BitStream_GetBitsLeft(bits) < 14) return MLO_ERROR_DECODER_INVALID_DATA;
		config->core_coder_delay = MLO_BitStream_ReadBits(bits, 14);
    } else {
        config->core_coder_delay = 0;
    }
    if (MLO_BitStream_GetBitsLeft(bits) < 1) return MLO_ERROR_DECODER_INVALID_DATA;
	MLO_BitStream_ReadBit(bits); /* extensionFlag */ 
	if (config->channel_configuration == MLO_CHANNEL_CONFIG_NONE) {		
		/*program_config_element (); */
	}		

    return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|   MLO_DecoderConfig_Parse
+---------------------------------------------------------------------*/
MLO_Result
MLO_DecoderConfig_Parse(const unsigned char* encoded, 
                        MLO_Size             encoded_size, 
                        MLO_DecoderConfig*   config)
{
    MLO_Result    result;
    MLO_BitStream bits;
    MLO_BitStream_Construct(&bits, encoded_size);
    MLO_BitStream_SetData(&bits, encoded, encoded_size);

    /* default config */
    MLO_SetMemory(config, 0, sizeof(*config));

	result = GetAudioObjectType(&bits, &config->object_type);
    if (MLO_FAILED(result)) return result;

    if (MLO_BitStream_GetBitsLeft(&bits) < 4) return MLO_ERROR_DECODER_INVALID_DATA;
	config->sampling_frequency_index = MLO_BitStream_ReadBits(&bits, 4);
	if (config->sampling_frequency_index == 0xF) {
        if (MLO_BitStream_GetBitsLeft(&bits) < 24) return MLO_ERROR_DECODER_INVALID_DATA;
		MLO_BitStream_ReadBits(&bits, 24);
	}
    if (MLO_BitStream_GetBitsLeft(&bits) < 4) return MLO_ERROR_DECODER_INVALID_DATA;
	config->channel_configuration = MLO_BitStream_ReadBits(&bits, 4);

	if (config->object_type == MLO_OBJECT_TYPE_SBR) {
		config->extension.object_type = config->object_type;
		config->extension.sbr_present = MLO_TRUE;
        if (MLO_BitStream_GetBitsLeft(&bits) < 4) return MLO_ERROR_DECODER_INVALID_DATA;
		config->sampling_frequency_index = MLO_BitStream_ReadBits(&bits, 4);;
        if (config->sampling_frequency_index == 0xF) {
            if (MLO_BitStream_GetBitsLeft(&bits) < 24) return MLO_ERROR_DECODER_INVALID_DATA;
			MLO_BitStream_ReadBits(&bits, 24);
        }
		result = GetAudioObjectType(&bits, &config->object_type);
        if (MLO_FAILED(result)) return result;
	} else {
   	    config->extension.sbr_present = MLO_FALSE;
		config->extension.object_type = 0;
        config->extension.sampling_frequency_index = 0;
	}
	switch (config->object_type) {
        case MLO_OBJECT_TYPE_AAC_MAIN:
        case MLO_OBJECT_TYPE_AAC_LC:
        case MLO_OBJECT_TYPE_AAC_SSR:
        case MLO_OBJECT_TYPE_AAC_LTP:
        case MLO_OBJECT_TYPE_AAC_SCALABLE:
        case MLO_OBJECT_TYPE_TWINVQ:
        case MLO_OBJECT_TYPE_ER_AAC_LC:
        case MLO_OBJECT_TYPE_ER_AAC_LTP:
        case MLO_OBJECT_TYPE_ER_AAC_SCALABLE:
        case MLO_OBJECT_TYPE_ER_TWINVQ:
        case MLO_OBJECT_TYPE_ER_BSAC:
        case MLO_OBJECT_TYPE_ER_AAC_LD:
            result = GetGASpecificInfo(&bits, config);
            if (MLO_FAILED(result)) return result;
            break;

        default:
            break;
    }

    return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|   MLO_DecoderConfig_GetChannelCount
+---------------------------------------------------------------------*/
MLO_Cardinal
MLO_DecoderConfig_GetChannelCount(const MLO_DecoderConfig* config)
{
    switch (config->channel_configuration) {
        case MLO_CHANNEL_CONFIG_MONO: return 1;
        case MLO_CHANNEL_CONFIG_STEREO: return 2;
        case MLO_CHANNEL_CONFIG_STEREO_PLUS_CENTER: return 3;
        case MLO_CHANNEL_CONFIG_STEREO_PLUS_CENTER_PLUS_REAR_MONO: return 4;
        case MLO_CHANNEL_CONFIG_FIVE: return 5;
        case MLO_CHANNEL_CONFIG_FIVE_PLUS_ONE: return 6;
        case MLO_CHANNEL_CONFIG_SEVEN_PLUS_ONE: return 8;
        default: return 0;
    }
}

/*----------------------------------------------------------------------
|       MLO_Decoder_Create
+---------------------------------------------------------------------*/
MLO_Result 
MLO_Decoder_Create(const MLO_DecoderConfig* config,
                   MLO_Decoder**            decoder)
{
    MLO_Result  result = MLO_SUCCESS;
    MLO_Boolean se_flag = MLO_FALSE;
    MLO_Boolean fb_flag = MLO_FALSE;

    /* check parameters */
    if (config == NULL || decoder == NULL) return MLO_ERROR_INVALID_PARAMETERS;
   
    /* default return value */
    *decoder = NULL;

    /* check that the config is supported */
    if (config->object_type != MLO_OBJECT_TYPE_AAC_LC ||
        config->frame_length_flag != MLO_FALSE        ||
        MLO_DecoderConfig_GetChannelCount(config) == 0) {
        return MLO_ERROR_DECODER_UNSUPPORTED_CONFIG;
    }

    /* allocate the decoder */
    *decoder = (MLO_Decoder*) MLO_AllocateZeroMemory (sizeof (MLO_Decoder));
    if (*decoder == NULL) result = MLO_ERROR_OUT_OF_MEMORY;

    if (MLO_SUCCEEDED (result)) {
        (*decoder)->config = *config;
        result = MLO_BitStream_Construct(&(*decoder)->bitstream, MLO_DECODER_MAX_FRAME_SIZE);
    }

    if (MLO_SUCCEEDED (result)) {
        result = MLO_SyntacticElements_Init (
           &(*decoder)->syntactic_elements
        );
    }

    if (MLO_SUCCEEDED (result)) {
        se_flag = MLO_TRUE;
        result = MLO_FilterBank_Init (&(*decoder)->filter_bank);
    }

    if (MLO_SUCCEEDED (result)) {
        fb_flag = MLO_TRUE;

        /*** To do ***/
    }

    if (MLO_FAILED (result)) {
        if (fb_flag) {
            MLO_FilterBank_Restore (
                &(*decoder)->filter_bank
            );
        }

        if (se_flag) {
            MLO_SyntacticElements_Restore (
                &(*decoder)->syntactic_elements
            );
        }

        if (*decoder != NULL) {
            MLO_BitStream_Destruct(&(*decoder)->bitstream);
            MLO_FreeMemory (*decoder);
            *decoder = NULL;
        }
    }

    return (result);
}

/*----------------------------------------------------------------------
|       MLO_Decoder_Destroy
+---------------------------------------------------------------------*/
MLO_Result 
MLO_Decoder_Destroy(MLO_Decoder* decoder)
{
    MLO_BitStream_Destruct(&decoder->bitstream);
    MLO_FilterBank_Restore (&decoder->filter_bank);
    MLO_SyntacticElements_Restore (&decoder->syntactic_elements);

    MLO_FreeMemory (decoder);

    return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|       MLO_Decoder_GetStatus
+---------------------------------------------------------------------*/
MLO_Result 
MLO_Decoder_GetStatus(MLO_Decoder* decoder, MLO_DecoderStatus** status)
{
    *status = &decoder->status;
    return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|       MLO_Decoder_DecodeFrame
+---------------------------------------------------------------------*/
MLO_Result 
MLO_Decoder_DecodeFrame(MLO_Decoder*      decoder, 
                        const MLO_Byte*   frame,
                        MLO_Size          frame_size,
                        MLO_SampleBuffer* buffer)
{

     /*** To do: call somewhere MLO_SyntacticElements_SetNbrChn ().
      It allocates memory, so we'd better not to put it in this
      function. ***/

    MLO_Result result = MLO_SUCCESS;

    /* check parameters */
    if (decoder == NULL || frame == NULL || buffer == NULL) {
        return MLO_ERROR_INVALID_PARAMETERS;
    }
    if (frame_size >= decoder->bitstream.buffer_size) {
        return MLO_ERROR_OUT_OF_RANGE;
    }

    /* setup the bitstream */
    MLO_BitStream_SetData(&decoder->bitstream, frame, frame_size);

    if (MLO_SUCCEEDED (result)) {
        MLO_SyntacticElements_StartNewFrame (
            &decoder->syntactic_elements,
            decoder->config.sampling_frequency_index
        );

        /* analyze the config to setup the buffer */
        {
            MLO_SampleFormat format;
            format.type = MLO_SAMPLE_TYPE_INTERLACED_SIGNED_16;
            format.sample_rate = MLO_SamplingFreq_table[decoder->config.sampling_frequency_index];
            format.channel_count = MLO_DecoderConfig_GetChannelCount(&decoder->config);
            format.bits_per_sample = 16;
            MLO_SampleBuffer_SetFormat(buffer, &format);
        }
        result = MLO_SampleBuffer_SetSampleCount(buffer, MLO_DEFS_FRAME_LEN_LONG);

        /* Decode frame content */
        result = MLO_Decoder_DecodeFrameContent (decoder, &decoder->bitstream, buffer);
     }

     if (MLO_SUCCEEDED (result)) {
         /* update our status */
         decoder->status.frame_count++;
         /*MLO_Int64_Add_Int32(decoder->status.sample_count, 
         buffer->sample_count);*/
         /*** To do: update flags ***/
     } else {
         MLO_SampleBuffer_SetSampleCount(buffer, 0);
     }

     return (result);
}

/*----------------------------------------------------------------------
|       MLO_Decoder_Reset
+---------------------------------------------------------------------*/
MLO_Result 
MLO_Decoder_Reset(MLO_Decoder* decoder)
{
    /* reset some of the decoder state */
    decoder->status.frame_count  = 0;
    /*decoder->status.sample_count = 0;*/

    return MLO_SUCCESS;
}

/*
==============================================================================
Name: MLO_Decoder_DecodeFrameContent
Description:
   Decode frame after buffer initialisation.
Input/output parameters:
	- decoder_ptr: this
	- frame_ptr: Frame basic information and input data bitstream to be
      decoded. Bitstream position is advanced to the end of the decoded data.
	- buffer_ptr: Buffer where to store decoded sound. Buffer should have been
      initialised before call.
Returns: MLO_SUCCESS if ok
==============================================================================
*/

MLO_Result  
MLO_Decoder_DecodeFrameContent (
   MLO_Decoder *        decoder_ptr, 
   MLO_BitStream *      bit_ptr,
   MLO_SampleBuffer *   buffer_ptr
)
{
   MLO_Result result = MLO_SUCCESS;

   MLO_ASSERT (decoder_ptr != 0);
   MLO_ASSERT (bit_ptr     != 0);
   MLO_ASSERT (buffer_ptr  != 0);

   /* Converts compressed bitstream into formated data */
   result = MLO_SyntacticElements_Decode (
      &decoder_ptr->syntactic_elements,
      bit_ptr
   );

   /* Finishes spectral processing */
   if (MLO_SUCCEEDED (result))
   {
      result = MLO_SyntacticElements_FinishSpectralProc (
         &decoder_ptr->syntactic_elements
      );
   }

   /* Converts signal from frequency to time domain and finishes processing */
   if (MLO_SUCCEEDED (result))
   {
      result = MLO_SyntacticElements_ConvertSpectralToTime (
         &decoder_ptr->syntactic_elements,
         &decoder_ptr->filter_bank
      );
   }

   /* To output buffer */
   if (MLO_SUCCEEDED (result))
   {
		MLO_SyntacticElements_SendToOutput (
         &decoder_ptr->syntactic_elements,
			buffer_ptr
      );
   }

   return (result);
}
