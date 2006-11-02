/*****************************************************************
|
|    Melo - MP4 to PCM Decoder
|
|    Copyright 2003-2006 Gilles Boccon-Gibod & Julien Boeuf
|
|
|    This file is part of Melo.
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

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "Ap4.h"
#include "Melo.h"
 
/*----------------------------------------------------------------------
|   PrintUsageAndExit
+---------------------------------------------------------------------*/
static void
PrintUsageAndExit()
{
    fprintf(stderr, 
            "MeloDecode\n" 
            "\n\nusage: melodecode [options] <input> <output>\n");
    exit(1);
}

/*----------------------------------------------------------------------
|   WriteSamples
+---------------------------------------------------------------------*/
static void
WriteSamples(AP4_Track* track, AP4_ByteStream* output)
{
    // get the sample description
    AP4_SampleDescription* desc = track->GetSampleDescription(0);
    if (desc == NULL) {
        fprintf(stderr, "ERROR: unable to get sample description\n");
        return;
    }
    AP4_MpegSampleDescription* mpeg_desc;
    if (desc->GetType() != AP4_SampleDescription::TYPE_MPEG) {
        fprintf(stderr, "ERROR: this track does not contain MPEG audio data\n");
        return;
    }
    mpeg_desc = dynamic_cast<AP4_MpegSampleDescription*>(desc);
    if (!mpeg_desc) {
        fprintf(stderr, "ERROR: sample description is not of the right type\n");
        return;
    }
    printf("    Stream Type: %s\n", AP4_MpegSampleDescription::GetStreamTypeString(mpeg_desc->GetStreamType()));
    printf("    Object Type: %s\n", AP4_MpegSampleDescription::GetObjectTypeString(mpeg_desc->GetObjectTypeId()));
    printf("    Max Bitrate: %d\n", mpeg_desc->GetMaxBitrate());
    printf("    Avg Bitrate: %d\n", mpeg_desc->GetAvgBitrate());
    printf("    Buffer Size: %d\n", mpeg_desc->GetBufferSize());
    if (mpeg_desc->GetStreamType() != AP4_AUDIO_STREAM_TYPE) {
        fprintf(stderr, "ERROR: unsupported stream type\n");
        return;
    }
    if (mpeg_desc->GetObjectTypeId() != AP4_MPEG2_AAC_AUDIO_LC_OTI &&
        mpeg_desc->GetObjectTypeId() != AP4_MPEG4_AUDIO_OTI) {
        fprintf(stderr, "ERROR: unsupported object type\n");
        return;
    }
    if (desc->GetFormat() != AP4_SAMPLE_FORMAT_MP4A) {
        fprintf(stderr, "ERROR: the sample format is not 'mp4a'\n");
        return;
    }

    AP4_AudioSampleDescription* audio_desc = dynamic_cast<AP4_AudioSampleDescription*>(desc);
    if (!audio_desc) {
        fprintf(stderr, "ERROR: sample description is not of the right type\n");
        return;
    }
    printf("    Sample Rate: %d\n", audio_desc->GetSampleRate());
    printf("    Sample Size: %d\n", audio_desc->GetSampleSize());
    printf("    Channels:    %d\n", audio_desc->GetChannelCount());

    AP4_Sample     sample;
    AP4_DataBuffer data;
    AP4_Ordinal    index = 0;
    MLO_DecoderConfig decoder_config = {
        MLO_OBJECT_TYPE_AAC_LC,
        MLO_SAMPLING_FREQ_44100 // FIXME: this should not be hardcoded
    };

    MLO_Decoder* decoder = NULL;
    MLO_Result result = MLO_Decoder_Create(&decoder_config, &decoder);
    if (MLO_FAILED(result)) {
        fprintf(stderr, "ERROR: failed to created MLO_Decoder (%d)\n", result);
        goto end;
    }

    MLO_SampleBuffer* pcm_buffer = NULL;
    result = MLO_SampleBuffer_Create(0, &pcm_buffer);
    if (MLO_FAILED(result)) goto end;

    while (AP4_SUCCEEDED(track->ReadSample(index, sample, data))) {
        result = MLO_Decoder_DecodeFrame(decoder, data.GetData(), data.GetDataSize(), pcm_buffer);
        printf("MLO_Decoder_DecodeFrame return %d\n", result);
        output->Write(MLO_SampleBuffer_GetSamples(pcm_buffer), MLO_SampleBuffer_GetSize(pcm_buffer));
	    index++;
    }

end:
    if (pcm_buffer) MLO_SampleBuffer_Destroy(pcm_buffer);
    if (decoder) MLO_Decoder_Destroy(decoder);
}

/*----------------------------------------------------------------------
|   main
+---------------------------------------------------------------------*/
int
main(int argc, char** argv)
{
    if (argc < 3) {
        PrintUsageAndExit();
    }
    
    // parse command line
    char** args = argv+1;

	// create the input stream
    AP4_ByteStream* input = 
        new AP4_FileByteStream(*args++,
                               AP4_FileByteStream::STREAM_MODE_READ);

	// create the output stream
    AP4_ByteStream* output =
        new AP4_FileByteStream(*args++,
                               AP4_FileByteStream::STREAM_MODE_WRITE);

	// open the file
    AP4_File* input_file = new AP4_File(*input);   

    // get the movie
    AP4_SampleDescription* sample_description;
    AP4_Track* audio_track;
    AP4_Movie* movie = input_file->GetMovie();
    if (movie == NULL) {
        fprintf(stderr, "ERROR: no movie in file\n");
        goto end;
    }

    // get the audio track
    audio_track = movie->GetTrack(AP4_Track::TYPE_AUDIO);
    if (audio_track == NULL) {
        fprintf(stderr, "ERROR: no audio track found\n");
        goto end;
    }

    // check that the track is of the right type
    sample_description = audio_track->GetSampleDescription(0);
    if (sample_description == NULL) {
        fprintf(stderr, "ERROR: unable to parse sample description\n");
        goto end;
    }

    // show info
    AP4_Debug("Audio Track:\n");
    AP4_Debug("  duration: %ld ms\n", audio_track->GetDurationMs());
    AP4_Debug("  sample count: %ld\n", audio_track->GetSampleCount());

    switch (sample_description->GetType()) {
        case AP4_SampleDescription::TYPE_MPEG:
            WriteSamples(audio_track, output);
            break;

        default:
            fprintf(stderr, "ERROR: unsupported sample type\n");
            break;
    }

end:
    delete input_file;
    input->Release();
    output->Release();

    return 0;
}

