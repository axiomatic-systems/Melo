#ifndef _MLO_IMDCT_H_
#define _MLO_IMDCT_H_



/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/

#include "MloFft.h"
#include "MloFloat.h"
#include "MloTypes.h"



/*----------------------------------------------------------------------
|       Types
+---------------------------------------------------------------------*/



typedef struct MLO_Imdct
{
   MLO_Float      buffer [2] [MLO_DEFS_FRAME_LEN_LONG];
   MLO_Fft        fft;
}  MLO_Imdct;



/*----------------------------------------------------------------------
|       Function prototypes
+---------------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



MLO_Result  MLO_Imdct_Init (MLO_Imdct *imdct_ptr);
void  MLO_Imdct_Restore (MLO_Imdct *imdct_ptr);

void  MLO_Imdct_Process (MLO_Imdct *imdct_ptr, MLO_Float x_ptr [], const MLO_Float f_ptr [], int len);

/* The following functions are public only for testing & debugging purpose. */
void  MLO_Imdct_ComputeDct4 (MLO_Imdct *imdct_ptr, MLO_Float dest_ptr [], const MLO_Float src_ptr [], int len);
void  MLO_Imdct_ComputeDct2 (MLO_Imdct *imdct_ptr, MLO_Float dest_ptr [], const MLO_Float src_ptr [], int len);



#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* _MLO_IMDCT_H_ */
