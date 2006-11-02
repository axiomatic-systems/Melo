/*****************************************************************
|
|      File: MloAdtsParser.h
|
|      Melo - ADTS Parser
|
|      (c) 2004 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file
 * Melo - Frames
 */

#ifndef _MLO_ADTS_PARSER_H_
#define _MLO_ADTS_PARSER_H_

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "MloTypes.h"

/*----------------------------------------------------------------------
|       types
+---------------------------------------------------------------------*/
typedef struct MLO_AdtsParser MLO_AdtsParser;

/*----------------------------------------------------------------------
|       function prototypes
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
    
MLO_Result MLO_AdtsParser_Create(MLO_AdtsParser** parser);
MLO_Result MLO_AdrsParser_Destroy(MLO_AdtsParser* parser);
MLO_Result MLO_AdtsParser_Feed(MLO_AdtsParser* parser, 
                               MLO_ByteBuffer  buffer, 
                               MLO_Size*       size,
                               MLO_Flags       flags);
MLO_Result MLO_AdtsParser_FindFrame(MLO_AdtsParser* parser,
                                    MLO_FrameData*  frame);
MLO_Result MLO_AdtsParser_Skip(MLO_AdtsParser* parser,
                               MLO_Size        bytes);
unsigned int   MLO_AdtsParser_GetBytesFree (const MLO_AdtsParser * parser_ptr);
unsigned int   MLO_AdtsParser_GetBytesAvailable (const MLO_AdtsParser * parser_ptr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MLO_ADTS_PARSER_H_ */
