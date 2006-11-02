/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/



#include "MloBitStream.h"
#include "MloDebug.h"
#include "MloElementDse.h"



/*----------------------------------------------------------------------
|       Functions
+---------------------------------------------------------------------*/



/*
==============================================================================
Name: MLO_ElementDse_decode
Description:
   Decodes and skips the Data Stream Element.
Input/output parameters:
	- bit_ptr: Input bitstream to decode
Returns:
   MLO_SUCCESS if ok
   MLO_FAILURE for a bitstream error
==============================================================================
*/

MLO_Result	MLO_ElementDse_Decode (MLO_BitStream *bit_ptr)
{
   MLO_Result     result = MLO_SUCCESS;
   MLO_Boolean    byte_align_flag;
   int            count;

	MLO_ASSERT (bit_ptr != 0);

   MLO_BitStream_ReadBits (bit_ptr, 4);   /* Ignores element_instance_tag */
   byte_align_flag = (MLO_Boolean) MLO_BitStream_ReadBit (bit_ptr);
   count = MLO_BitStream_ReadBits (bit_ptr, 8);
   if (count == 255)
   {
      count += MLO_BitStream_ReadBits (bit_ptr, 8);
   }

   if (byte_align_flag)
   {
      result = MLO_BitStream_ByteAlign (bit_ptr);
   }

   if (MLO_SUCCEEDED (result))
   {
      MLO_BitStream_SkipBits (bit_ptr, count * 8);
   }

	return (result);
}



