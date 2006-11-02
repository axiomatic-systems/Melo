/*****************************************************************
|
|      File: MloDebug.h
|
|      Melo - Debug Support
|
|      (c) 2004 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file
 * Header file for debug & logging
 */

#ifndef _MLO_DEBUG_H_
#define _MLO_DEBUG_H_

/*----------------------------------------------------------------------
|    includes
+---------------------------------------------------------------------*/

#include "MloConfig.h"

#if defined(MLO_CONFIG_HAVE_ASSERT_H)
#include <assert.h>
#endif

/*----------------------------------------------------------------------
|    Macros
+---------------------------------------------------------------------*/

/* Check a constant expression to make the compiler fail if false.
   Requires a ";" at the end */
#define  MLO_CHECK_CST(name, cond)  typedef int MLO_CheckCst_##name [(cond) ? 1 : -1]

/*----------------------------------------------------------------------
|    prototypes
+---------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern void MLO_Debug(const char* format, ...);
#define MLO_ASSERT(_x) assert(_x)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MLO_DEBUG_H_ */
