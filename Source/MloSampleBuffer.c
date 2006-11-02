/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "MloSampleBuffer.h"
#include "MloResults.h"

/*----------------------------------------------------------------------
|   types
+---------------------------------------------------------------------*/
struct MLO_SampleBuffer {
    MLO_Byte*        buffer;
    MLO_Size         buffer_size;
    MLO_Size         data_size;
    MLO_SampleFormat format;
};

/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
#define MLO_DATA_BUFFER_EXTRA_GROW_SPACE 256

/*----------------------------------------------------------------------
|   MLO_SampleBuffer_Create
+---------------------------------------------------------------------*/
MLO_Result 
MLO_SampleBuffer_Create(MLO_Size size, MLO_SampleBuffer** buffer)
{
    /* allocate the object */
    *buffer = MLO_AllocateZeroMemory(sizeof(MLO_SampleBuffer));
    if (*buffer == NULL) return MLO_ERROR_OUT_OF_MEMORY;

    /* allocate the buffer */
    if (size) {
        (*buffer)->buffer_size = size;
        (*buffer)->buffer = MLO_AllocateMemory(size);
        if ((*buffer)->buffer == NULL) {
            MLO_FreeMemory((void*)(*buffer));
            return MLO_ERROR_OUT_OF_MEMORY;
        }
    }
    return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|   MLO_SampleBuffer_Destroy
+---------------------------------------------------------------------*/
MLO_Result 
MLO_SampleBuffer_Destroy(MLO_SampleBuffer* self)
{
    /* free the buffer */
    MLO_FreeMemory((void*)self->buffer);

    /* free the object */
    MLO_FreeMemory((void*)self);

    return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|   MLO_SampleBuffer_GetFormat
+---------------------------------------------------------------------*/
const MLO_SampleFormat* 
MLO_SampleBuffer_GetFormat(const MLO_SampleBuffer* self)
{
    return &self->format;
}

/*----------------------------------------------------------------------
|   MLO_SampleBuffer_SetFormat
+---------------------------------------------------------------------*/
void
MLO_SampleBuffer_SetFormat(MLO_SampleBuffer*       self, 
                           const MLO_SampleFormat* format)
{
    self->format = *format;
}

/*----------------------------------------------------------------------
|   MLO_SampleBuffer_ReallocateBuffer
+---------------------------------------------------------------------*/
static MLO_Result
MLO_SampleBuffer_ReallocateBuffer(MLO_SampleBuffer* self, MLO_Size size)
{
    MLO_Byte* new_buffer;

    /* check that the existing data fits */
    if (self->data_size > size) return MLO_ERROR_INVALID_PARAMETERS;

    /* allocate a new buffer */
    new_buffer = (MLO_Byte*)MLO_AllocateMemory(size);
    if (new_buffer == NULL) return MLO_ERROR_OUT_OF_MEMORY;

    /* copy the contents of the previous buffer, if any */
    if (self->buffer && self->data_size) {
        MLO_CopyMemory(new_buffer, self->buffer, self->data_size);
    }

    /* destroy the previous buffer */
    MLO_FreeMemory((void*)self->buffer);

    /* use the new buffer */
    self->buffer = new_buffer;
    self->buffer_size = size;

    return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|   MLO_SampleBuffer_Reserve
+---------------------------------------------------------------------*/
MLO_Result 
MLO_SampleBuffer_Reserve(MLO_SampleBuffer* self, MLO_Size size)
{
    if (self->buffer_size >= size) return MLO_SUCCESS;
    
    /* try doubling the size */
    {
        MLO_Size new_size = self->buffer_size*2;
        if (new_size < size) new_size = size + MLO_DATA_BUFFER_EXTRA_GROW_SPACE;
        return MLO_SampleBuffer_ReallocateBuffer(self, new_size);
    }
}

/*----------------------------------------------------------------------
|   MLO_SampleBuffer_SetDataSize
+---------------------------------------------------------------------*/
MLO_Result 
MLO_SampleBuffer_SetDataSize(MLO_SampleBuffer* self, MLO_Size size)
{
    if (size > self->buffer_size) {
        /* the buffer is too small, we need to reallocate it */
        MLO_SampleBuffer_ReallocateBuffer(self, size);
    }
    self->data_size = size;

    return MLO_SUCCESS;
}

/*----------------------------------------------------------------------
|   MLO_SampleBuffer_SetSampleCount
+---------------------------------------------------------------------*/
MLO_Result 
MLO_SampleBuffer_SetSampleCount(MLO_SampleBuffer* self,
                                MLO_Cardinal      sample_count)
{
    MLO_Size size =  self->format.channel_count*
                    (self->format.bits_per_sample/8) *
                     sample_count;
    return MLO_SampleBuffer_SetDataSize(self, size);
}

/*----------------------------------------------------------------------
|   MLO_SampleBuffer_GetSampleCount
+---------------------------------------------------------------------*/
MLO_Cardinal 
MLO_SampleBuffer_GetSampleCount(const MLO_SampleBuffer* self)
{
    unsigned int bytes_per_sample = self->format.channel_count*
                                   (self->format.bits_per_sample/8);
    return bytes_per_sample?self->data_size/bytes_per_sample:0;
}

/*----------------------------------------------------------------------
|   MLO_SampleBuffer_GetSamples
+---------------------------------------------------------------------*/
const void*  
MLO_SampleBuffer_GetSamples(const MLO_SampleBuffer* self)
{
    return self->buffer;
}

/*----------------------------------------------------------------------
|   MLO_SampleBuffer_UseSamples
+---------------------------------------------------------------------*/
void* 
MLO_SampleBuffer_UseSamples(MLO_SampleBuffer* self)
{
    return self->buffer;
}

/*----------------------------------------------------------------------
|   MLO_SampleBuffer_GetSize
+---------------------------------------------------------------------*/
MLO_Size   
MLO_SampleBuffer_GetSize(const MLO_SampleBuffer* self)
{
    return self->data_size;
}
