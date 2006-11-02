/*****************************************************************
|
|      File: MloFrame.h
|
|      Melo - Frames
|
|      (c) 2004 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file
 * Melo - Frames
 */

#ifndef _MLO_FRAME_H_
#define _MLO_FRAME_H_

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "MloSamplingFreq.h"
#include "MloBitStream.h"
#include "MloTypes.h"

/*----------------------------------------------------------------------
|       types
+---------------------------------------------------------------------*/
typedef enum {
    MLO_AAC_STANDARD_MPEG2,
    MLO_AAC_STANDARD_MPEG4
} MLO_AacStandard;

typedef enum {
    MLO_AAC_PROFILE_MAIN,
    MLO_AAC_PROFILE_LC,
    MLO_AAC_PROFILE_SSR,
    MLO_AAC_PROFILE_LTP
} MLO_AacProfile;

typedef struct {
    MLO_AacStandard standard;
    MLO_AacProfile  profile;
    MLO_SamplingFreq_Index
                    sampling_frequency_index;
    unsigned long   sampling_frequency;
    unsigned int    channel_configuration;
    unsigned int    frame_length;            /* With headers and check, bytes */
} MLO_FrameInfo;

typedef struct {
    MLO_BitStream* source;
    MLO_FrameInfo  info;
} MLO_FrameData;

#endif /* _MLO_FRAME_H_ */
