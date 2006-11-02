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
|       constants
+---------------------------------------------------------------------*/


/*----------------------------------------------------------------------
|       types
+---------------------------------------------------------------------*/
struct MLO_Decoder {
    MLO_DecoderConfig      config;
    MLO_DecoderStatus      status;
    MLO_SyntacticElements  syntactic_elements;
    MLO_FilterBank         filter_bank;
    struct {
        void*    samples;
        MLO_Size size;
    } buffer;
};

/*----------------------------------------------------------------------
|       Prototypes
+---------------------------------------------------------------------*/
MLO_Result  
MLO_Decoder_DecodeFrameContent (
   MLO_Decoder *        decoder_ptr, 
   MLO_BitStream *      bits_ptr,
   MLO_SampleBuffer *   buffer_ptr
);

/*----------------------------------------------------------------------
|       MLO_Decoder_Create
+---------------------------------------------------------------------*/
MLO_Result 
MLO_Decoder_Create(const MLO_DecoderConfig* config,
                   MLO_Decoder**            decoder_ptr_ptr)
{
   MLO_Result     result = MLO_SUCCESS;
   MLO_Boolean    se_flag = MLO_FALSE;
   MLO_Boolean    fb_flag = MLO_FALSE;

   *decoder_ptr_ptr =
      (MLO_Decoder*) MLO_AllocateZeroMemory (sizeof (MLO_Decoder));
   if (*decoder_ptr_ptr == NULL)
   {
      result = MLO_ERROR_OUT_OF_MEMORY;
   }

   if (MLO_SUCCEEDED (result)) {
      (*decoder_ptr_ptr)->config = *config;
   }

   if (MLO_SUCCEEDED (result))
   {
      result = MLO_SyntacticElements_Init (
         &(*decoder_ptr_ptr)->syntactic_elements
      );
   }

   if (MLO_SUCCEEDED (result))
   {
      se_flag = MLO_TRUE;
      result = MLO_FilterBank_Init (&(*decoder_ptr_ptr)->filter_bank);
   }

   if (MLO_SUCCEEDED (result))
   {
      fb_flag = MLO_TRUE;

      /*** To do ***/

   }

   /*** To do: initialize status (flags) ***/

   if (MLO_FAILED (result))
   {
      if (fb_flag)
      {
         MLO_FilterBank_Restore (
            &(*decoder_ptr_ptr)->filter_bank
         );
      }

      if (se_flag)
      {
         MLO_SyntacticElements_Restore (
            &(*decoder_ptr_ptr)->syntactic_elements
         );
      }

      if (*decoder_ptr_ptr != NULL)
      {
         MLO_FreeMemory (*decoder_ptr_ptr);
         *decoder_ptr_ptr = NULL;
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
   /* free the internal buffer if we had one */
   if (decoder->buffer.samples != NULL)
   {
      MLO_FreeMemory (decoder->buffer.samples);
   }

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
|       MLO_Decoder_AllocateBuffer
+---------------------------------------------------------------------*/
static MLO_Result 
MLO_Decoder_AllocateBuffer(MLO_Decoder* decoder, MLO_Size required)
{
    /* return now if we already have enough space */
    if (decoder->buffer.size >= required) {
        return MLO_SUCCESS;
    }

    /* allocate enough space */
    if (decoder->buffer.samples != NULL) {
        MLO_FreeMemory(decoder->buffer.samples);
    }
    decoder->buffer.samples = MLO_AllocateMemory(required);
    if (decoder->buffer.samples == NULL) {
        decoder->buffer.size = 0;
        return MLO_ERROR_OUT_OF_MEMORY;
    }
    decoder->buffer.size = required;

    return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|       MLO_Decoder_DecodeFrame
+---------------------------------------------------------------------*/
MLO_Result 
MLO_Decoder_DecodeFrame(MLO_Decoder*      decoder, 
                        MLO_BitStream*    bits,
                        MLO_SampleBuffer* buffer)
{

   /*** To do: call somewhere MLO_SyntacticElements_SetNbrChn ().
      It allocates memory, so we'd better not to put it in this
      function. ***/

   MLO_Result     result = MLO_SUCCESS;
   MLO_Size       required;
   int            bytes_per_samples;

   MLO_ASSERT (decoder != 0);
   MLO_ASSERT (buffer != 0);

   /* analyze the frame to setup the buffer */
   /* NOTE: assume 1024 for now */
   /* NOTE: assume 16 bits/sample for now */
   /*** To do: change this ***/
   buffer->format.type = MLO_SAMPLE_TYPE_INTERLACED_SIGNED_16;
   buffer->format.sample_rate = MLO_SamplingFreq_table[decoder->config.sampling_frequency_index];
   buffer->format.channel_count = 2; // FIXME: frame->info.channel_configuration;
   buffer->format.bits_per_sample = 16;
   buffer->sample_count = MLO_DEFS_FRAME_LEN_LONG;

   /* check the buffer */
   bytes_per_samples =
      (buffer->format.bits_per_sample + (CHAR_BIT - 1)) / CHAR_BIT;
   required =   buffer->sample_count
              * buffer->format.channel_count
              * bytes_per_samples;
   if (buffer->samples == NULL)
   {
      /* no buffer was supplied, use the internal one */
      result = MLO_Decoder_AllocateBuffer(decoder, required);
      if (MLO_SUCCEEDED (result))
      {
         buffer->samples = decoder->buffer.samples;
      }
   }
   else
   {
      /* a buffer was supplied, check that it is large enough */
      if (buffer->size < required)
      {
         buffer->size = required;
         result = MLO_ERROR_BUFFER_TOO_SMALL;
      }
   }

   if (MLO_SUCCEEDED (result))
   {
      MLO_SyntacticElements_StartNewFrame (
         &decoder->syntactic_elements,
         decoder->config.sampling_frequency_index
      );

      /* Decode frame content */
      result = MLO_Decoder_DecodeFrameContent (decoder, bits, buffer);
   }

   if (MLO_SUCCEEDED (result))
   {
      /* update our status */
      decoder->status.frame_count++;
      /*MLO_Int64_Add_Int32(decoder->status.sample_count, 
      buffer->sample_count);*/
      /*** To do: update flags ***/

      /* update the buffer info */
      buffer->size =   buffer->sample_count
                     * buffer->format.channel_count
                     * bytes_per_samples;
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
