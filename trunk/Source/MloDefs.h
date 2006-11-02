/*****************************************************************
|
|      File: MloDefs.h
|
|      Melo - Common Definitions
|
|      (c) 2004 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file
 * Header file for common definitions
 */

#ifndef _MLO_DEFS_H_
#define _MLO_DEFS_H_

/*----------------------------------------------------------------------
|       constants
+---------------------------------------------------------------------*/
#ifndef __cplusplus
#ifndef NULL
#define NULL ((void*)0)
#endif
#endif /* __cplusplus */

/* Fixed frame length */
enum {   MLO_DEFS_FRAME_LEN_LONG    = 1024 };
enum {   MLO_DEFS_FRAME_LEN_SHORT   = MLO_DEFS_FRAME_LEN_LONG / 8 };

/* Maximum number of windows and window groups */
enum {   MLO_DEFS_MAX_NBR_WINDOWS   = 8   };
enum {   MLO_DEFS_MAX_NBR_WIN_GRP   = MLO_DEFS_MAX_NBR_WINDOWS   };

/* Maximum number of Scale Window Bands (related to MLO_IcsInfo data) */
enum {   MLO_DEFS_MAX_NUM_SWB       = 51 };

/* Maximum number of channels that can be supported by the decoder */
enum {   MLO_DEFS_MAX_CHN           = 32 };

/* Numeric constants */
#define  MLO_DEFS_PI                (3.1415926535897932384626433832795)
#define  MLO_DEFS_SQRT2             (1.41421356237309514547462185873883)



#endif /* _MLO_DEFS_H_ */
