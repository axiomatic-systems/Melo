/*****************************************************************
|
|      File: MloResults.h
|
|      MLO - Result & Error Constants
|
|      (c) 2004 Gilles Boccon-Gibod
|      Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/
/** @file
 * MLO Result and Error codes
 */

#ifndef _MLO_ERRORS_H_
#define _MLO_ERRORS_H_

/*----------------------------------------------------------------------
|    includes
+---------------------------------------------------------------------*/

/*----------------------------------------------------------------------
|    error codes
+---------------------------------------------------------------------*/
#define MLO_SUCCESS      0
#define MLO_FAILURE      (-1)

#define MLO_FAILED(result)       ((result) != MLO_SUCCESS)
#define MLO_SUCCEEDED(result)    ((result) == MLO_SUCCESS)

/* Generic Errors */
#define MLO_ERROR_BASE_GENERIC            (-20000)
#define MLO_ERROR_INVALID_PARAMETERS      (MLO_ERROR_BASE_GENERIC-0)
#define MLO_ERROR_OUT_OF_MEMORY           (MLO_ERROR_BASE_GENERIC-1)

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
|    import some Atomix error codes
+---------------------------------------------------------------------*/

#endif /* _MLO_ERRORS_H_ */
