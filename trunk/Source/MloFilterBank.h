/*
Does filter bank processing and block switching

Ref:
4.6.11
*/

#ifndef _MLO_FILTER_BANK_H_
#define _MLO_FILTER_BANK_H_



/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/



#include "MloImdct.h"
#include "MloIndivChnStream.h"
#include "MloDefs.h"
#include "MloTypes.h"



/*----------------------------------------------------------------------
|       Types
+---------------------------------------------------------------------*/



typedef struct MLO_FilterBank
{
   MLO_Imdct      imdct;
   MLO_Float      tmp_buf [MLO_DEFS_FRAME_LEN_LONG * 2]; /* For IMDCT result and overlapped data */
   MLO_Float      tmp_win [MLO_DEFS_FRAME_LEN_LONG * 2]; /* Windowed data */
}  MLO_FilterBank;



/*----------------------------------------------------------------------
|       Function prototypes
+---------------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



MLO_Result  MLO_FilterBank_Init (MLO_FilterBank *fb_ptr);
void  MLO_FilterBank_Restore (MLO_FilterBank *fb_ptr);

void  MLO_FilterBank_ConvertSpectralToTime (MLO_FilterBank *fb_ptr, MLO_IndivChnStream *ics_ptr);



#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* _MLO_FILTER_BANK_H_ */
