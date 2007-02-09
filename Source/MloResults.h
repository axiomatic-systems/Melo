/*****************************************************************
|
|    MLO - Result & Error Constants
|
|    Copyright 2004-2006 Axiomatic Systems LLC
|
|    This file is part of Melo (Melo AAC Decoder).
|
|    Unless you have obtained Melo under a difference license,
|    this version of Melo is Melo|GPL.
|    Melo|GPL is free software; you can redistribute it and/or modify
|    it under the terms of the GNU General Public License as published by
|    the Free Software Foundation; either version 2, or (at your option)
|    any later version.
|
|    Melo|GPL is distributed in the hope that it will be useful,
|    but WITHOUT ANY WARRANTY; without even the implied warranty of
|    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|    GNU General Public License for more details.
|
|    You should have received a copy of the GNU General Public License
|    along with Melo|GPL; see the file COPYING.  If not, write to the
|    Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
|    02111-1307, USA.
|
 ****************************************************************/
/** @file
 * Results and Error codes
 */

#ifndef _MLO_ERRORS_H_
#define _MLO_ERRORS_H_

/*----------------------------------------------------------------------
|    includes
+---------------------------------------------------------------------*/

/*----------------------------------------------------------------------
|    error codes
+---------------------------------------------------------------------*/
/** Result code indicating that a call was successful */
#define MLO_SUCCESS      0

/** Error: an unspecififed error has occurred. */
#define MLO_FAILURE      (-1)

#define MLO_FAILED(result)       ((result) != MLO_SUCCESS)
#define MLO_SUCCEEDED(result)    ((result) == MLO_SUCCESS)

/* Generic Errors */
#define MLO_ERROR_BASE_GENERIC            (-20000)
#define MLO_ERROR_INVALID_PARAMETERS      (MLO_ERROR_BASE_GENERIC-0)
#define MLO_ERROR_OUT_OF_MEMORY           (MLO_ERROR_BASE_GENERIC-1)
#define MLO_ERROR_OUT_OF_RANGE            (MLO_ERROR_BASE_GENERIC-2)
#define MLO_ERROR_INVALID_DATA            (MLO_ERROR_BASE_GENERIC-3)

/* Decoder errors */
#define MLO_ERROR_BASE_DECODER            (-20100)

/* BitStream errors */
#define MLO_ERROR_BASE_BITSTREAM          (-20200)

/* IcsInfo errors */
#define MLO_ERROR_BASE_ICS_INFO           (-20300)

/* IndivChnPool errors */
#define MLO_ERROR_BASE_INDIV_CHN_POOL     (-20400)

/* IndivChnStream errors */
#define MLO_ERROR_BASE_INDIV_CHN_STREAM   (-20500)

/* Huffman errors */
#define MLO_ERROR_BASE_HUFFMAN            (-20600)

/* ElementFil errors */
#define MLO_ERROR_BASE_ELEMENT_FIL        (-20700)

/* ElementFil errors */
#define MLO_ERROR_BASE_SCALE_FACTOR       (-20800)

/* SyntacticElements errors */
#define MLO_ERROR_BASE_SYNTACTIC_ELEMENTS (-20900)

/*----------------------------------------------------------------------
|    macros
+---------------------------------------------------------------------*/
#if defined(MLO_DEBUG)
#define MLO_CHECK_ARGS(_x) MLO_ASSERT(_x)
#else
#define MLO_CHECK_ARGS(_x) if (!(_x)) return MLO_ERROR_INVALID_PARAMETERS
#endif

#define MLO_CHECK_DATA(_x) if (!(_x)) return MLO_ERROR_INVALID_DATA

#endif /* _MLO_ERRORS_H_ */
