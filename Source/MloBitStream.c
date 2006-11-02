/*****************************************************************
|
|      File: MloBitStream.c
|
|      Melo - Bit Streams
|
|      (c) 2004 Gilles Boccon-Gibod
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
|       MLO_BitStream_AllocateBuffer
+---------------------------------------------------------------------*/
MLO_Result
MLO_BitStream_AllocateBuffer(MLO_BitStream* bits)
{
    bits->buffer = 
        (unsigned char*)MLO_AllocateMemory(MLO_BITSTREAM_BUFFER_SIZE);
    if (bits->buffer == NULL) return MLO_ERROR_OUT_OF_MEMORY;

    MLO_BitStream_Reset(bits);

    return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|       MLO_BitStream_FreeBuffer
+---------------------------------------------------------------------*/
MLO_Result
MLO_BitStream_FreeBuffer(MLO_BitStream* bits)
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
    bits->in          = 0;
    bits->out         = 0;
    bits->bits_cached = 0;
    bits->cache       = 0;
    bits->flags       = 0;

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

/*----------------------------------------------------------------------
|       MLO_BitStream_GetContiguousBytesFree
+---------------------------------------------------------------------*/
unsigned int 
MLO_BitStream_GetContiguousBytesFree(const MLO_BitStream* bits)
{
    return 
        (bits->in < bits->out) ?
        (bits->out - bits->in - 1) :
        (bits->out == 0 ? 
            (MLO_BITSTREAM_BUFFER_SIZE - bits->in-1) :
            (MLO_BITSTREAM_BUFFER_SIZE - bits->in));
}

/*----------------------------------------------------------------------
|       MLO_BitStream_GetBytesFree
+---------------------------------------------------------------------*/
unsigned int 
MLO_BitStream_GetBytesFree(const MLO_BitStream* bits)
{
    return  
        (bits->in < bits->out) ? 
        (bits->out - bits->in - 1) : 
        (MLO_BITSTREAM_BUFFER_SIZE  + (bits->out - bits->in) - 1);
}

/*----------------------------------------------------------------------+
|        MLO_BitStream_WriteBytes
+----------------------------------------------------------------------*/
MLO_Result
MLO_BitStream_WriteBytes(MLO_BitStream*       bits, 
                         const unsigned char* bytes, 
                         unsigned int         byte_count)
{
    /* check parameters */
    if (byte_count == 0) return MLO_SUCCESS;
    if (bytes == NULL) return MLO_ERROR_INVALID_PARAMETERS;

    /* check that we have enough space */
    if (MLO_BitStream_GetBytesFree(bits) < byte_count) {
        return MLO_ERROR_NOT_ENOUGH_FREE_BUFFER;
    }

    /* write the bytes */
    if (bits->in < bits->out) {
        MLO_CopyMemory(bits->buffer+bits->in, bytes, byte_count);
        MLO_BITSTREAM_POINTER_ADD(bits->in, byte_count);
    } else {
        unsigned int chunk = MLO_BITSTREAM_BUFFER_SIZE - bits->in;
        if (chunk > byte_count) chunk = byte_count;

        MLO_CopyMemory(bits->buffer+bits->in, bytes, chunk);
        MLO_BITSTREAM_POINTER_ADD(bits->in, chunk);

        if (chunk != byte_count) {
            MLO_CopyMemory(bits->buffer+bits->in, 
                           bytes+chunk, byte_count-chunk);
            MLO_BITSTREAM_POINTER_ADD(bits->in, byte_count-chunk);
        }
    }

    return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|       MLO_BitStream_GetContiguousBytesAvailable
+---------------------------------------------------------------------*/
unsigned int 
MLO_BitStream_GetContiguousBytesAvailable(const MLO_BitStream* bits)
{
    return 
        (bits->out <= bits->in) ? 
        (bits->in - bits->out) :
        (MLO_BITSTREAM_BUFFER_SIZE - bits->out);
}

/*----------------------------------------------------------------------
|       MLO_BitStream_GetBytesAvailable
+---------------------------------------------------------------------*/
unsigned int 
MLO_BitStream_GetBytesAvailable(const MLO_BitStream* bits)
{
    return 
        (bits->out <= bits->in) ? 
        (bits->in - bits->out) :
        (bits->in + (MLO_BITSTREAM_BUFFER_SIZE - bits->out));
}

/*----------------------------------------------------------------------+
|        MLO_BitStream_ReadBytes
+----------------------------------------------------------------------*/
MLO_Result
MLO_BitStream_ReadBytes(MLO_BitStream* bits, 
                        unsigned char* bytes, 
                        unsigned int   byte_count)
{
   if (byte_count == 0 || bytes == NULL) {
      return MLO_ERROR_INVALID_PARAMETERS;
   }

   /* Gets bytes from the cache */
   MLO_BitStream_ByteAlign (bits);
   while (bits->bits_cached > 0 && byte_count > 0) {
      *bytes = MLO_BitStream_ReadBits (bits, 8);
      ++ bytes;
      -- byte_count;
   }

   /* Get other bytes */
   if (byte_count > 0) {
      if (bits->out < bits->in) {
         MLO_CopyMemory (bytes, bits->buffer + bits->out, byte_count);
         MLO_BITSTREAM_POINTER_ADD(bits->out, byte_count);
      } else {
         unsigned int chunk = MLO_BITSTREAM_BUFFER_SIZE - bits->out;
         if (chunk >= byte_count) {
            chunk = byte_count;
         }

         MLO_CopyMemory(bytes, bits->buffer+bits->out, chunk);
         MLO_BITSTREAM_POINTER_ADD(bits->out, chunk);

         if (chunk != byte_count) {
            MLO_CopyMemory(bytes+chunk, 
                           bits->buffer+bits->out, 
                           byte_count-chunk);
            MLO_BITSTREAM_POINTER_ADD(bits->out, byte_count-chunk);
         }
      }
   }

   return MLO_SUCCESS;
}

/*----------------------------------------------------------------------+
|        MLO_BitStream_PeekBytes
+----------------------------------------------------------------------*/
MLO_Result
MLO_BitStream_PeekBytes(const MLO_BitStream* bits, 
                        unsigned char* bytes, 
                        unsigned int   byte_count)
{
   int bits_cached_byte;

   if (byte_count == 0 || bytes == NULL) {
      return MLO_ERROR_INVALID_PARAMETERS;
   }

   /* Gets bytes from the cache */
   bits_cached_byte = bits->bits_cached & ~7;
   while (bits_cached_byte > 0 && byte_count > 0) {
      *bytes = (bits->cache >> bits_cached_byte) & 0xFF;
      ++ bytes;
      -- byte_count;
      bits_cached_byte -= 8;
   }

   /* Get other bytes */
   if (byte_count > 0) {
      if (bits->in > bits->out) {
         MLO_CopyMemory (bytes, bits->buffer + bits->out, byte_count);
      } else {
         unsigned int out = bits->out;
         unsigned int chunk = MLO_BITSTREAM_BUFFER_SIZE - out;
         if (chunk >= byte_count) {
            chunk = byte_count;
         }

         MLO_CopyMemory(bytes, bits->buffer+out, chunk);
         MLO_BITSTREAM_POINTER_ADD(out, chunk);

         if (chunk != byte_count) {
            MLO_CopyMemory(bytes+chunk, 
                           bits->buffer+out, 
                           byte_count-chunk);
         }
      }
   }

   return MLO_SUCCESS;
}

/*----------------------------------------------------------------------+
|        MLO_BitStream_SkipBytes
+----------------------------------------------------------------------*/
MLO_Result
MLO_BitStream_SkipBytes(MLO_BitStream* bits, int byte_count)
{
    MLO_BITSTREAM_POINTER_ADD(bits->out, byte_count);
    return MLO_SUCCESS;
}
