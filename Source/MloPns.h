/*
Perceptual Noise Substitution (PNS)

Ref:
4.6.13
*/

#ifndef _MLO_PNS_H_
#define _MLO_PNS_H_



/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/



#include "MloElementCpe.h"
#include "MloIndivChnStream.h"



/*----------------------------------------------------------------------
|       Function prototypes
+---------------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



void  MLO_Pns_ProcessSingle (MLO_IndivChnStream *ics_ptr);
void  MLO_Pns_ProcessPair (MLO_ElementCpe *cpe_ptr);



#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* _MLO_PNS_H_ */
