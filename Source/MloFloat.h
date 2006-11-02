#ifndef _MLO_FLOAT_H_
#define _MLO_FLOAT_H_



/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/



#include "MloConfig.h"



#if ! defined (MLO_CONFIG_FIXED)

#include "MloFloatFpu.h"

#else    /* MLO_CONFIG_FIXED */

#include "MloFloatFix.h"

#endif   /* MLO_CONFIG_FIXED */



#endif   /* _MLO_FLOAT_H_ */
