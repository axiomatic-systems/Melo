/*****************************************************************
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
   MLO_SAMPLE_TYPE_INTERLACED_SIGNED = 0
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
