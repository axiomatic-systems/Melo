/*****************************************************************
|
|      File: MloUtils.c
|
|      Melo - Utils
|
|      (c) 2004 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|       includes
+---------------------------------------------------------------------*/
#include "MloConfig.h"
#include "MloUtils.h"
#include "MloResults.h"

/*----------------------------------------------------------------------
|    MLO_BytesFromInt32Be
+---------------------------------------------------------------------*/
void 
MLO_BytesFromInt32Be(unsigned char* buffer, unsigned long value)
{
    buffer[0] = (unsigned char)(value>>24) & 0xFF;
    buffer[1] = (unsigned char)(value>>16) & 0xFF;
    buffer[2] = (unsigned char)(value>> 8) & 0xFF;
    buffer[3] = (unsigned char)(value    ) & 0xFF;
}

/*----------------------------------------------------------------------
|    MLO_BytesFromInt16Be
+---------------------------------------------------------------------*/
void 
MLO_BytesFromInt16Be(unsigned char* buffer, unsigned short value)
{
    buffer[0] = (unsigned char)((value>> 8) & 0xFF);
    buffer[1] = (unsigned char)((value    ) & 0xFF);
}

/*----------------------------------------------------------------------
|    MLO_BytesToInt32Be
+---------------------------------------------------------------------*/
unsigned long 
MLO_BytesToInt32Be(const unsigned char* buffer)
{
    return 
        ( ((unsigned long)buffer[0])<<24 ) |
        ( ((unsigned long)buffer[1])<<16 ) |
        ( ((unsigned long)buffer[2])<<8  ) |
        ( ((unsigned long)buffer[3])     );
}

/*----------------------------------------------------------------------
|    MLO_BytesToInt16Be
+---------------------------------------------------------------------*/
unsigned short 
MLO_BytesToInt16Be(const unsigned char* buffer)
{
    return 
        ( ((unsigned short)buffer[0])<<8  ) |
        ( ((unsigned short)buffer[1])     );
}

/*----------------------------------------------------------------------
|    MLO_BytesFromInt32Le
+---------------------------------------------------------------------*/
void 
MLO_BytesFromInt32Le(unsigned char* buffer, unsigned long value)
{
    buffer[3] = (unsigned char)(value>>24) & 0xFF;
    buffer[2] = (unsigned char)(value>>16) & 0xFF;
    buffer[1] = (unsigned char)(value>> 8) & 0xFF;
    buffer[0] = (unsigned char)(value    ) & 0xFF;
}

/*----------------------------------------------------------------------
|    MLO_BytesFromInt16Le
+---------------------------------------------------------------------*/
void 
MLO_BytesFromInt16Le(unsigned char* buffer, unsigned short value)
{
    buffer[1] = (unsigned char)((value>> 8) & 0xFF);
    buffer[0] = (unsigned char)((value    ) & 0xFF);
}

/*----------------------------------------------------------------------
|    MLO_BytesToInt32Le
+---------------------------------------------------------------------*/
unsigned long 
MLO_BytesToInt32Le(const unsigned char* buffer)
{
    return 
        ( ((unsigned long)buffer[3])<<24 ) |
        ( ((unsigned long)buffer[2])<<16 ) |
        ( ((unsigned long)buffer[1])<<8  ) |
        ( ((unsigned long)buffer[0])     );
}

/*----------------------------------------------------------------------
|    MLO_BytesToInt16Le
+---------------------------------------------------------------------*/
unsigned short 
MLO_BytesToInt16Le(const unsigned char* buffer)
{
    return 
        ( ((unsigned short)buffer[1])<<8  ) |
        ( ((unsigned short)buffer[0])     );
}
