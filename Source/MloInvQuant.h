/*
Does inverse quantizing of spectral data

Ref:
4.6.1, 4.6.2
*/

#ifndef _MLO_INV_QUANT_H_
#define _MLO_INV_QUANT_H_



/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/



#include "MloIndivChnStream.h"



/*----------------------------------------------------------------------
|       Function prototypes
+---------------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



void  MLO_InvQuant_ProcessChannel (MLO_IndivChnStream *ics_ptr);



#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* _MLO_INV_QUANT_H_ */
