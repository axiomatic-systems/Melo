/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/
#include "MloBitStream.h"
#include "MloDebug.h"
#include "MloElementPce.h"

/*----------------------------------------------------------------------
|       Prototypes
+---------------------------------------------------------------------*/
static void  MLO_ElementPce_ReadTaggedElementArray (MLO_ElementPce_TaggedElt tag_elt_ptr [], MLO_BitStream *bit_ptr, int nbr_elt);

/*----------------------------------------------------------------------
|       Functions
+---------------------------------------------------------------------*/

/*
==============================================================================
Name: MLO_ElementPce_Decode
Description:
   Decode a Program Config Element
Output parameters:
	- pce_ptr: Object to build with the decoded input stream
Input/output parameters:
	- bits_ptr: Input bitstream to decode
Returns:
   MLO_SUCCESS if ok
   MLO_FAILURE for a bitstream error, a non-LC profile or an invalide
      sampling rate index.
==============================================================================
*/

MLO_Result	MLO_ElementPce_Decode (MLO_ElementPce *pce_ptr, MLO_BitStream *bit_ptr)
{
   MLO_Result     result = MLO_SUCCESS;
   int            index;

	MLO_ASSERT (pce_ptr != 0);
	MLO_ASSERT (bit_ptr != 0);

   pce_ptr->element_instance_tag = MLO_BitStream_ReadBits (bit_ptr, 4);
   pce_ptr->object_type = (MLO_AacProfile) MLO_BitStream_ReadBits (bit_ptr, 2);
   if (pce_ptr->object_type != MLO_AAC_PROFILE_LC)
   {
      result = MLO_FAILURE;
   }

   if (MLO_SUCCEEDED (result))
   {
      pce_ptr->sampling_frequency_index =
         (MLO_SamplingFreq_Index) MLO_BitStream_ReadBits (bit_ptr, 4);
      if (pce_ptr->sampling_frequency_index >= MLO_SAMPLING_FREQ_INDEX_NBR_VALID)
      {
         result = MLO_FAILURE;
      }
   }

   if (MLO_SUCCEEDED (result))
   {
      pce_ptr->num_front_channel_elements = MLO_BitStream_ReadBits (bit_ptr, 4);
      pce_ptr->num_side_channel_elements = MLO_BitStream_ReadBits (bit_ptr, 4);
      pce_ptr->num_back_channel_elements = MLO_BitStream_ReadBits (bit_ptr, 4);
      pce_ptr->num_lfe_channel_elements = MLO_BitStream_ReadBits (bit_ptr, 2);
      pce_ptr->num_assoc_data_elements = MLO_BitStream_ReadBits (bit_ptr, 3);
      pce_ptr->num_valid_cc_elements = MLO_BitStream_ReadBits (bit_ptr, 4);

      pce_ptr->mono_mixdown_present =
         (MLO_Boolean) MLO_BitStream_ReadBit (bit_ptr);
      if (pce_ptr->mono_mixdown_present)
      {
         pce_ptr->mono_mixdown_element_number =
            MLO_BitStream_ReadBits (bit_ptr, 4);
      }

      pce_ptr->stereo_mixdown_present =
         (MLO_Boolean) MLO_BitStream_ReadBit (bit_ptr);
      if (pce_ptr->stereo_mixdown_present)
      {
         pce_ptr->stereo_mixdown_element_number =
            MLO_BitStream_ReadBits (bit_ptr, 4);
      }

      pce_ptr->matrix_mixdown_idx_present =
         (MLO_Boolean) MLO_BitStream_ReadBit (bit_ptr);
      if (pce_ptr->matrix_mixdown_idx_present)
      {
         pce_ptr->matrix_mixdown_idx = MLO_BitStream_ReadBits (bit_ptr, 2);
         pce_ptr->pseudo_surround_enable =
            (MLO_Boolean) MLO_BitStream_ReadBit (bit_ptr);
      }

      MLO_ElementPce_ReadTaggedElementArray (
         pce_ptr->front_element_arr,
         bit_ptr,
         pce_ptr->num_front_channel_elements
      );

      MLO_ElementPce_ReadTaggedElementArray (
         pce_ptr->side_element_arr,
         bit_ptr,
         pce_ptr->num_side_channel_elements
      );

      MLO_ElementPce_ReadTaggedElementArray (
         pce_ptr->back_element_arr,
         bit_ptr,
         pce_ptr->num_back_channel_elements
      );

      for (index = 0; index < pce_ptr->num_lfe_channel_elements; ++index)
      {
         pce_ptr->lfe_element_tag_arr [index] =
            MLO_BitStream_ReadBits (bit_ptr, 4);
      }

      for (index = 0; index < pce_ptr->num_assoc_data_elements; ++index)
      {
         pce_ptr->assoc_data_element_tag_arr [index] =
            MLO_BitStream_ReadBits (bit_ptr, 4);
      }

      for (index = 0; index < pce_ptr->num_valid_cc_elements; ++index)
      {
         pce_ptr->cc_element_arr [index].is_ind_sw_flag =
            (MLO_Boolean) MLO_BitStream_ReadBit (bit_ptr);
         pce_ptr->cc_element_arr [index].tag =
            MLO_BitStream_ReadBits (bit_ptr, 4);
      }

      result = MLO_BitStream_ByteAlign (bit_ptr);
   }

   if (MLO_SUCCEEDED (result))
   {
      int            comment_field_bytes = MLO_BitStream_ReadByte (bit_ptr);
      result = MLO_BitStream_SkipBytes (bit_ptr, comment_field_bytes);
   }

	return (result);
}



/*
==============================================================================
Name: MLO_ElementPce_ReadTaggedElementArray
Description:
   Decode an array of MLO_ElementPce_TaggedElt from the bitstream.
Input parameters:
	- nbr_elt: Number of elements to decode, >= 0, limited by the array size
Output parameters:
	- tag_elt_ptr: start of the array to fill
Input/output parameters:
	- bits_ptr: Input bitstream to decode
==============================================================================
*/

void	MLO_ElementPce_ReadTaggedElementArray (MLO_ElementPce_TaggedElt tag_elt_ptr [], MLO_BitStream *bit_ptr, int nbr_elt)
{
   int            index;

	MLO_ASSERT (tag_elt_ptr != 0);
	MLO_ASSERT (bit_ptr != 0);
	MLO_ASSERT (nbr_elt >= 0);

   for (index = 0; index < nbr_elt; ++index)
   {
      tag_elt_ptr [index].is_cpe_flag =
         (MLO_Boolean) MLO_BitStream_ReadBit (bit_ptr);
      tag_elt_ptr [index].tag = MLO_BitStream_ReadBits (bit_ptr, 4);
   }
}



