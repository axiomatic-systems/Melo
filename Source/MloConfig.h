/*****************************************************************
|
|      File: MloConfig.h
|
|      Melo - Configuration
|
|      (c) 2004 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file
 * Melo Configuration
 */

#ifndef _MLO_CONFIG_H_
#define _MLO_CONFIG_H_

/*----------------------------------------------------------------------
|    defaults
+---------------------------------------------------------------------*/
#define MLO_CONFIG_HAVE_STD_C
#define MLO_CONFIG_HAVE_STDLIB_H
#define MLO_CONFIG_HAVE_STRING_H
#define MLO_CONFIG_HAVE_CTYPE_H
#define MLO_CONFIG_HAVE_MATH_H
#define MLO_CONFIG_HAVE_ASSERT_H
#define MLO_CONFIG_HAVE_LIMITS_H

/*----------------------------------------------------------------------
|       base types
+---------------------------------------------------------------------*/
typedef int              MLO_Int32;
typedef unsigned int     MLO_UInt32;
typedef short            MLO_Int16;
typedef unsigned short   MLO_UInt16;
typedef signed char      MLO_Int8;
typedef unsigned char    MLO_UInt8;
typedef int              MLO_Int;

/*----------------------------------------------------------------------
|    platform specifics
+---------------------------------------------------------------------*/
/* Windows 32 */
#if defined(WIN32)
#if !defined(STRICT)
#define STRICT
#define inline __inline
#endif

#define vsnprintf _vsnprintf

#endif

/* Microsoft C/C++ Compiler */
#if defined(_MSC_VER)
#define MLO_CONFIG_HAVE_INT64
#define MLO_CONFIG_INT64_TYPE __int64
#endif

/* QNX */
#if defined(__QNX__)
#endif

/* define/undef for fixed/floating point arithmetic */
#undef   MLO_CONFIG_FIXED

/*----------------------------------------------------------------------
|    compiler specifics
+---------------------------------------------------------------------*/
/* GCC */
#if defined(__GNUC__)
#define MLO_COMPILER_UNUSED(p) (void)p
#define MLO_CONFIG_HAVE_INT64
#define MLO_CONFIG_INT64_TYPE long long
#else
#define MLO_COMPILER_UNUSED(p) 
#endif

/*----------------------------------------------------------------------
|    standard C runtime
+---------------------------------------------------------------------*/
#if defined (MLO_CONFIG_HAVE_STD_C)
#define MLO_CONFIG_HAVE_MALLOC
#define MLO_CONFIG_HAVE_CALLOC
#define MLO_CONFIG_HAVE_REALLOC
#define MLO_CONFIG_HAVE_FREE
#define MLO_CONFIG_HAVE_MEMCPY
#define MLO_CONFIG_HAVE_MEMMOVE
#define MLO_CONFIG_HAVE_MEMSET
#endif /* MLO_CONFIG_HAS_STD_C */

#if defined (MLO_CONFIG_HAVE_STRING_H)
#define MLO_CONFIG_HAVE_STRCMP
#define MLO_CONFIG_HAVE_STRNCMP
#define MLO_CONFIG_HAVE_STRDUP
#define MLO_CONFIG_HAVE_STRLEN
#define MLO_CONFIG_HAVE_STRCPY
#define MLO_CONFIG_HAVE_STRNCPY
#endif /* MLO_CONFIG_HAVE_STRING_H */

#if defined (MLO_CONFIG_HAVE_MATH_H)
#define MLO_CONFIG_HAVE_POW
#define MLO_CONFIG_HAVE_SQRT
#define MLO_CONFIG_HAVE_COS
#define MLO_CONFIG_HAVE_SIN
#endif /* MLO_CONFIG_HAVE_MATH_H */

#endif /* _MLO_CONFIG_H_ */
