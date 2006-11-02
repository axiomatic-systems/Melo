/*****************************************************************
|
|      File: FloBitStream.h
|
|      Melo - Bit Streams
|
|      (c) 2004 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file
 * Melo - Bit Streams
 */

#ifndef _MLO_BIT_STREAM_H_
#define _MLO_BIT_STREAM_H_

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "MloConfig.h"
#include "MloTypes.h"
#include "MloResults.h"

/*----------------------------------------------------------------------
|       constants
+---------------------------------------------------------------------*/

/* the max frame size we can handle */
#define MLO_BITSTREAM_BUFFER_SIZE  8192

/* flags */
#define MLO_BITSTREAM_FLAG_EOS 0x01

/* error codes */
#define MLO_ERROR_NOT_ENOUGH_DATA        (MLO_ERROR_BASE_BITSTREAM - 0)
#define MLO_ERROR_CORRUPTED_BITSTREAM    (MLO_ERROR_BASE_BITSTREAM - 1)
#define MLO_ERROR_NOT_ENOUGH_FREE_BUFFER (MLO_ERROR_BASE_BITSTREAM - 2)

/*----------------------------------------------------------------------
|       types helpers
+---------------------------------------------------------------------*/
/* use long by default */
typedef unsigned int MLO_BitsWord;
#define MLO_WORD_BITS  32
#define MLO_WORD_BYTES 4

/*----------------------------------------------------------------------
|       types
+---------------------------------------------------------------------*/
typedef struct MLO_BitStream
{
    unsigned char* buffer;
    unsigned int   in;
    unsigned int   out;
    MLO_BitsWord   cache;
    unsigned int   bits_cached;
    unsigned int   flags;
} MLO_BitStream;

/*----------------------------------------------------------------------
|       prototypes
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
    
MLO_Result   MLO_BitStream_AllocateBuffer(MLO_BitStream* bits);
MLO_Result   MLO_BitStream_FreeBuffer(MLO_BitStream* bits);
MLO_Result   MLO_BitStream_Reset(MLO_BitStream* bits);
unsigned int MLO_BitStream_GetContiguousBytesFree(const MLO_BitStream* bits);
unsigned int MLO_BitStream_GetBytesFree(const MLO_BitStream* bits);
MLO_Result   MLO_BitStream_WriteBytes(MLO_BitStream*       bits, 
                                      const unsigned char* bytes, 
                                      unsigned int         byte_count);
unsigned int MLO_BitStream_GetContiguousBytesAvailable(const MLO_BitStream* bits);
unsigned int MLO_BitStream_GetBytesAvailable(const MLO_BitStream* bits);
MLO_Result   MLO_BitStream_ReadBytes(MLO_BitStream* bits, 
                                     unsigned char* bytes, 
                                     unsigned int   byte_count);
MLO_Result   MLO_BitStream_PeekBytes(const MLO_BitStream* bits, 
                                     unsigned char* bytes, 
                                     unsigned int   byte_count);
MLO_Result   MLO_BitStream_SkipBytes(MLO_BitStream* bits, 
                                     int            byte_count);
MLO_Result   MLO_BitStream_ByteAlign(MLO_BitStream* bits);

#ifdef __cplusplus
}
#endif /* __cplusplus */
    
/*----------------------------------------------------------------------
|       macros
+---------------------------------------------------------------------*/
#define MLO_BIT_MASK(_n) ((1<<(_n))-1)

#define MLO_BITSTREAM_POINTER_VAL(offset) \
    ((offset)&(MLO_BITSTREAM_BUFFER_SIZE-1))

#define MLO_BITSTREAM_POINTER_OFFSET(pointer, offset) \
    (MLO_BITSTREAM_POINTER_VAL((pointer)+(offset)))

#define MLO_BITSTREAM_POINTER_ADD(pointer, offset) \
    ((pointer) = MLO_BITSTREAM_POINTER_OFFSET(pointer, offset))

/*
==============================================================================
Name: MLO_BitStream_ReadCache
Description:
   This is a private function, for internal use.
   Reads bytes to get cached bits.
Input parameters:
	- bits_ptr: Pointer on the BitStream structure
Returns: The cached bits
==============================================================================
*/

static inline MLO_BitsWord
MLO_BitStream_ReadCache (const MLO_BitStream* bits_ptr)
{
   unsigned int   pos = bits_ptr->out;
   MLO_BitsWord   cache;

#if MLO_WORD_BITS != 32
#error unsupported word size /* 64 and other word size not yet implemented */
#endif

   if (pos <= MLO_BITSTREAM_BUFFER_SIZE - MLO_WORD_BYTES)
   {
      unsigned char *   out_ptr = &bits_ptr->buffer [pos];
      cache =   (((MLO_BitsWord) out_ptr [0]) << 24)
              | (((MLO_BitsWord) out_ptr [1]) << 16)
              | (((MLO_BitsWord) out_ptr [2]) <<  8)
              | (((MLO_BitsWord) out_ptr [3])      );
   }
   else
   {
      unsigned char *   buf_ptr = bits_ptr->buffer;
      cache =   (((MLO_BitsWord) buf_ptr [                              pos    ]) << 24)
              | (((MLO_BitsWord) buf_ptr [MLO_BITSTREAM_POINTER_OFFSET (pos, 1)]) << 16)
              | (((MLO_BitsWord) buf_ptr [MLO_BITSTREAM_POINTER_OFFSET (pos, 2)]) <<  8)
              | (((MLO_BitsWord) buf_ptr [MLO_BITSTREAM_POINTER_OFFSET (pos, 3)])      );
   }

   return (cache);
}

/*----------------------------------------------------------------------
|       MLO_BitStream_ReadBits
+---------------------------------------------------------------------*/
static inline unsigned int
MLO_BitStream_ReadBits(MLO_BitStream* bits, unsigned int n)
{
    MLO_BitsWord   result;
    if (bits->bits_cached >= n) {
        /* we have enough bits in the cache to satisfy the request */
        bits->bits_cached -= n;
        result = (bits->cache >> bits->bits_cached) & MLO_BIT_MASK(n);
    } else {
        /* not enough bits in the cache */
        MLO_BitsWord word;

        /* read the next word */
        word = MLO_BitStream_ReadCache (bits);
        bits->out = MLO_BITSTREAM_POINTER_OFFSET(bits->out, MLO_WORD_BYTES);

        /* combine the new word and the cache, and update the state */
        {
            MLO_BitsWord cache = bits->cache & MLO_BIT_MASK(bits->bits_cached);
            n -= bits->bits_cached;
            bits->bits_cached = MLO_WORD_BITS - n;
            result = (word >> bits->bits_cached) | (cache << n);
            bits->cache = word;
        }
    }

    return result;
}

/*----------------------------------------------------------------------
|       MLO_BitStream_ReadBit
+---------------------------------------------------------------------*/
static inline unsigned int
MLO_BitStream_ReadBit(MLO_BitStream* bits)
{
    MLO_BitsWord   result;
    if (bits->bits_cached == 0) {
        /* the cache is empty */

        /* read the next word into the cache */
        bits->cache = MLO_BitStream_ReadCache (bits);
        bits->out = MLO_BITSTREAM_POINTER_OFFSET(bits->out, MLO_WORD_BYTES);
        bits->bits_cached = MLO_WORD_BITS - 1;

        /* return the first bit */
        result = bits->cache >> (MLO_WORD_BITS - 1);
    } else {
        /* get the bit from the cache */
        result = (bits->cache >> (--bits->bits_cached)) & 1;
    }
    return result;
}

/*----------------------------------------------------------------------
|       MLO_BitStream_PeekBits
+---------------------------------------------------------------------*/
static inline unsigned int
MLO_BitStream_PeekBits(const MLO_BitStream* bits, unsigned int n)
{
   /* we have enough bits in the cache to satisfy the request */
   if (bits->bits_cached >= n) {
      return (bits->cache >> (bits->bits_cached - n)) & MLO_BIT_MASK(n);
   } else {
      /* not enough bits in the cache */
      /* read the next word */
      MLO_BitsWord   word = MLO_BitStream_ReadCache (bits);

      /* combine the new word and the cache, and update the state */
      MLO_BitsWord   cache = bits->cache & MLO_BIT_MASK(bits->bits_cached);
      n -= bits->bits_cached;
      return (word >> (MLO_WORD_BITS - n)) | (cache << n);
   }
}

/*----------------------------------------------------------------------
|       MLO_BitStream_PeekBit
+---------------------------------------------------------------------*/
static inline unsigned int
MLO_BitStream_PeekBit(const MLO_BitStream* bits)
{
   /* the cache is empty */
   if (bits->bits_cached == 0) {
      /* read the next word into the cache */
      MLO_BitsWord   cache = MLO_BitStream_ReadCache (bits);

      /* return the first bit */
      return cache >> (MLO_WORD_BITS - 1);
   } else {
      /* get the bit from the cache */
      return (bits->cache >> (bits->bits_cached-1)) & 1;
   }
}

/*----------------------------------------------------------------------
|       MLO_BitStream_SkipBits
+---------------------------------------------------------------------*/
static inline void
MLO_BitStream_SkipBits(MLO_BitStream* bits, unsigned int n)
{
   if (n <= bits->bits_cached) {
      bits->bits_cached -= n;
   } else {
      n -= bits->bits_cached;
      while (n >= MLO_WORD_BITS) {
         bits->out = MLO_BITSTREAM_POINTER_OFFSET(bits->out, MLO_WORD_BYTES);
         n -= MLO_WORD_BITS;
      }
      if (n) {
         bits->cache = MLO_BitStream_ReadCache (bits);
         bits->bits_cached = MLO_WORD_BITS-n;
         bits->out = MLO_BITSTREAM_POINTER_OFFSET(bits->out, MLO_WORD_BYTES);
      } else {
         bits->bits_cached = 0;
         bits->cache = 0;
      }
   }
}

/*----------------------------------------------------------------------
|       MLO_BitStream_SkipBit
+---------------------------------------------------------------------*/
static inline void
MLO_BitStream_SkipBit(MLO_BitStream* bits)
{
   if (bits->bits_cached == 0) {
      bits->cache = MLO_BitStream_ReadCache (bits);
      bits->out = MLO_BITSTREAM_POINTER_OFFSET(bits->out, MLO_WORD_BYTES);
      bits->bits_cached = MLO_WORD_BITS - 1;
   } else {
      --bits->bits_cached;
   }
}

/*----------------------------------------------------------------------
|       MLO_BitStream_ReadByte
+---------------------------------------------------------------------*/
static inline unsigned char
MLO_BitStream_ReadByte(MLO_BitStream* bits)
{
   MLO_BitStream_SkipBits (bits, bits->bits_cached & 7);
   return (MLO_BitStream_ReadBits (bits, 8));
}

/*----------------------------------------------------------------------
|       MLO_BitStream_PeekByte
+---------------------------------------------------------------------*/
static inline unsigned char
MLO_BitStream_PeekByte(const MLO_BitStream* bits)
{
   int            extra_bits = bits->bits_cached & 7;
   int            data = MLO_BitStream_PeekBits (bits, extra_bits + 8);
   int            byte = data & 0xFF;

   return (byte);
}

#endif /* _MLO_BIT_STREAM_H_ */
