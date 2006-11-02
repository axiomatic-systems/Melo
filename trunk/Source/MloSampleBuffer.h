#ifndef _MLO_SAMPLE_BUFFER_H_
#define _MLO_SAMPLE_BUFFER_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "MloTypes.h"
#include "MloUtils.h"

/*----------------------------------------------------------------------
|    types
+---------------------------------------------------------------------*/
typedef struct MLO_SampleBuffer MLO_SampleBuffer;

typedef enum
{
   MLO_SAMPLE_TYPE_INTERLACED_SIGNED_16 = 0,
} MLO_SampleType;

typedef struct
{
   MLO_SampleType type;
   MLO_Cardinal   sample_rate;
   MLO_Cardinal   channel_count;
   MLO_Cardinal   bits_per_sample;
} MLO_SampleFormat;

/*----------------------------------------------------------------------
|   functions
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

MLO_Result 
MLO_SampleBuffer_Create(MLO_Size size, MLO_SampleBuffer** buffer);

MLO_Result 
MLO_SampleBuffer_Destroy(MLO_SampleBuffer* self);

const void* 
MLO_SampleBuffer_GetSamples(const MLO_SampleBuffer* self);

void* 
MLO_SampleBuffer_UseSamples(MLO_SampleBuffer* self);

MLO_Cardinal   
MLO_SampleBuffer_GetSampleCount(const MLO_SampleBuffer* self);

MLO_Size       
MLO_SampleBuffer_GetSize(const MLO_SampleBuffer* self);

const MLO_SampleFormat* 
MLO_SampleBuffer_GetFormat(const MLO_SampleBuffer* self);

void 
MLO_SampleBuffer_SetFormat(MLO_SampleBuffer* self, const MLO_SampleFormat* format);

MLO_Result 
MLO_SampleBuffer_SetSampleCount(MLO_SampleBuffer* self, MLO_Cardinal sample_count);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MLO_SAMPLE_BUFFER_H_ */
