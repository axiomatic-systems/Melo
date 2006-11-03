/*****************************************************************
|
|      File: MloBitStream.c
|
|      Melo - Bit Streams
|
|      (c) 2004-2006 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       For efficiency reasons, this bitstream library only handles
|       data buffers that are a power of 2 in size
+---------------------------------------------------------------------*/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "MloUtils.h"
#include "MloConfig.h"
#include "MloDebug.h"
#include "MloTypes.h"
#include "MloBitStream.h"
#include "MloFrame.h"

/*----------------------------------------------------------------------
|       MLO_BitStream_Construct
+---------------------------------------------------------------------*/
MLO_Result
MLO_BitStream_Construct(MLO_BitStream* bits, MLO_Size size)
{
    /* make the buffer size an integer number of words */
    size = MLO_WORD_BYTES*((size+MLO_WORD_BYTES-1)/MLO_WORD_BYTES);
    bits->buffer = (unsigned char*)MLO_AllocateMemory(size);
    bits->buffer_size = size;
    bits->data_size = 0;
    if (bits->buffer == NULL) return MLO_ERROR_OUT_OF_MEMORY;

    MLO_BitStream_Reset(bits);

    return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|       MLO_BitStream_Destruct
+---------------------------------------------------------------------*/
MLO_Result
MLO_BitStream_Destruct(MLO_BitStream* bits)
{
    MLO_BitStream_Reset(bits);
    if (bits->buffer != NULL) {
        MLO_FreeMemory(bits->buffer);
        bits->buffer = 0;
    }

    return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|       MLO_BitStream_Reset
+---------------------------------------------------------------------*/
MLO_Result
MLO_BitStream_Reset(MLO_BitStream* bits)
{
    bits->pos         = 0;
    bits->bits_cached = 0;
    bits->cache       = 0;

    return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|       MLO_BitStream_GetBitsLeft
+---------------------------------------------------------------------*/
MLO_Size   
MLO_BitStream_GetBitsLeft(MLO_BitStream* bits)
{
    return 8*(bits->data_size-bits->pos)+bits->bits_cached;
}

/*----------------------------------------------------------------------
|       MLO_BitStream_SetData
+---------------------------------------------------------------------*/
MLO_Result 
MLO_BitStream_SetData(MLO_BitStream*  bits, 
                      const MLO_Byte* data, 
                      MLO_Size        data_size)
{
    if (data_size > bits->buffer_size) {
        return MLO_ERROR_OUT_OF_RANGE;
    }
    if (data == NULL) return MLO_ERROR_INVALID_PARAMETERS;

    /* copy the data in the buffer */
    bits->data_size = data_size;
    MLO_CopyMemory(bits->buffer, data, data_size);

    /* fill the rest of the buffer with zeros */
    if (data_size != bits->buffer_size) {
        MLO_SetMemory(bits->buffer+data_size, 0, bits->buffer_size-data_size);
    }

    /* start from the beginning of the buffer */
    MLO_BitStream_Reset(bits);

    return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|       MLO_BitStream_ByteAlign
+---------------------------------------------------------------------*/
MLO_Result   
MLO_BitStream_ByteAlign(MLO_BitStream* bits)
{
    unsigned int to_flush = bits->bits_cached & 7;
    if (to_flush > 0) {
        MLO_BitStream_SkipBits(bits, to_flush);
    }
    return MLO_SUCCESS;
}
