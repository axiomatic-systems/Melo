#ifndef _MLO_FFT_H_
#define _MLO_FFT_H_



/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/



#include "MloDefs.h"
#include "MloFloat.h"
#include "MloTypes.h"



/*----------------------------------------------------------------------
|       Constants
+---------------------------------------------------------------------*/



enum {   MLO_FFT_BR_PACK         = 4   };
enum {   MLO_FFT_TABLE_LEN_BR_L  = MLO_DEFS_FRAME_LEN_LONG  / MLO_FFT_BR_PACK  };
enum {   MLO_FFT_TABLE_LEN_BR_S  = MLO_DEFS_FRAME_LEN_SHORT / MLO_FFT_BR_PACK  };

/* For tables */
enum {   MLO_FFT_TABLE_LEN_COS   = MLO_DEFS_FRAME_LEN_LONG >> 2   };



/*----------------------------------------------------------------------
|       Types
+---------------------------------------------------------------------*/



typedef struct MLO_Fft
{
   MLO_Float      buffer [MLO_DEFS_FRAME_LEN_LONG];
   MLO_Int16      table_br_l [MLO_FFT_TABLE_LEN_BR_L];   /* Bit-reverse table (only multiples of MLO_FFT_BR_PACK) */
   MLO_Int16      table_br_s [MLO_FFT_TABLE_LEN_BR_S];
}  MLO_Fft;



/*----------------------------------------------------------------------
|       Function prototypes
+---------------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



void  MLO_Fft_Init (MLO_Fft *fft_ptr);
void  MLO_Fft_Process (MLO_Fft *fft_ptr, MLO_Float x_ptr [], const MLO_Float f_ptr [], int len);



#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* _MLO_FFT_H_ */
