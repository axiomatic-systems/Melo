/*
Section data

Ref:
4.4.2.7, Table 4.46
4.5.2.3.2
*/



/*----------------------------------------------------------------------
|       Includes
+---------------------------------------------------------------------*/



#include "MloBitStream.h"
#include "MloDebug.h"
#include "MloHcb.h"
#include "MloIcsInfo.h"
#include "MloSectionData.h"
#include "MloUtils.h"



/*----------------------------------------------------------------------
|       Functions
+---------------------------------------------------------------------*/



/*
==============================================================================
Name: MLO_SectionData_Decode
Description:
   Decodes the section_data section.
   Ref:
      4.4.2.7, Table 4.46
      4.5.2.3.2
Input parameters:
	- sec_ptr: MLO_SectionData object to fill.
Input/output parameters:
	- bits_ptr: Input bitstream to decode
Output parameters:
   - sec_ptr: MLO_SectionData object to be decoded
==============================================================================
*/

void  MLO_SectionData_Decode (MLO_SectionData *sec_ptr, const MLO_IcsInfo *ics_ptr, MLO_BitStream *bit_ptr)
{
   int            sect_len_bits = 5;
   int            sect_esc_val;
   int            g;

	MLO_ASSERT (sec_ptr != 0);
	MLO_ASSERT (ics_ptr != 0);
	MLO_ASSERT (bit_ptr != 0);

   if (ics_ptr->window_sequence == MLO_ICS_INFO_WIN_EIGHT_SHORT_SEQUENCE)
   {
      sect_len_bits = 3;
   }
   sect_esc_val = (1 << sect_len_bits) - 1;

   for (g = 0; g < ics_ptr->num_window_groups; ++g)
   {
      int            k = 0;
      int            i = 0;
      while (k < ics_ptr->max_sfb)
      {
         int            sect_len = 0;
         int            sect_len_incr;
         int            sfb;
         int            end;

         MLO_ASSERT (i < MLO_ARRAY_SIZE (sec_ptr->sect_cb [0]));
         MLO_ASSERT (i < MLO_ARRAY_SIZE (sec_ptr->sect_start [0]));
         MLO_ASSERT (i < MLO_ARRAY_SIZE (sec_ptr->sect_end [0]));
         MLO_ASSERT (i < MLO_ARRAY_SIZE (sec_ptr->sect_cb [0]));

         /* aacSectionDataResilienceFlag not set */
         sec_ptr->sect_cb [g] [i] = MLO_BitStream_ReadBits (bit_ptr, 4);

         do
         {
            sect_len_incr = MLO_BitStream_ReadBits (bit_ptr, sect_len_bits);
            sect_len += sect_len_incr;
         }
         while (sect_len_incr == sect_esc_val);

         end = k + sect_len;
         MLO_ASSERT (k <= MLO_MAX_VAL_U (sec_ptr->sect_start [0] [0]));
         MLO_ASSERT (end <= MLO_MAX_VAL_U (sec_ptr->sect_end [0] [0]));
         sec_ptr->sect_start [g] [i] = k;
         sec_ptr->sect_end [g] [i] = end;

         for (sfb = k; sfb < end; ++sfb)
         {
            MLO_ASSERT (sfb < MLO_ARRAY_SIZE (sec_ptr->sfb_cb [0]));
            sec_ptr->sfb_cb [g] [sfb] = sec_ptr->sect_cb [g] [i];
         }

         k += sect_len;
         ++ i;
      }

      sec_ptr->num_sec [g] = i;
   }
}



/*
==============================================================================
Name: MLO_SectionData_IsIntensity
Description:
   Returns the state of a Scale Factor Band regarding intensity stereo
   decoding. This function applies only to the right channel of a pair,
   and only if they have a common ics_info.
   Ref:
      4.6.8.2.3
Input parameters:
	- sec_ptr: MLO_SectionData object containing information 
	- group: Window group, [0 ; ics_info.num_window_groups[
	- sfb: Scale Factor Band index, in [0 ; ics_info.max_sfb[
Returns:
   +1: Codebook is INTENSITY_HCB
   -1: Codebook is INTENSITY_HCB2
   0 : otherwise
==============================================================================
*/

int   MLO_SectionData_IsIntensity (const MLO_SectionData *sec_ptr, int group, int sfb)
{
   MLO_Hcb        c;
   int            ret_val = 0;

   MLO_ASSERT (sec_ptr != 0);
   MLO_ASSERT (group >= 0);
   MLO_ASSERT (group < MLO_ARRAY_SIZE (sec_ptr->sect_cb));
   MLO_ASSERT (sfb >= 0);
   MLO_ASSERT (sfb < MLO_ARRAY_SIZE (sec_ptr->sect_cb [0]));

   c = sec_ptr->sfb_cb [group] [sfb];
   if (c == MLO_HCB_INTENSITY_HCB)
   {
      ret_val = 1;
   }
   else if (c == MLO_HCB_INTENSITY_HCB2)
   {
      ret_val = -1;
   }

   return (ret_val);
}



/*
==============================================================================
Name: MLO_SectionData_IsNoise
Description:
   Test if a Scale Factor Band is noise.
   Ref:
      4.6.13.3
Input parameters:
	- sec_ptr: MLO_SectionData object containing information 
	- group: Window group, [0 ; ics_info.num_window_groups[
	- sfb: Scale Factor Band index, in [0 ; ics_info.max_sfb[
Returns:
   MLO_TRUE if Scale Factor Band is noise type.
==============================================================================
*/

MLO_Boolean MLO_SectionData_IsNoise (const MLO_SectionData *sec_ptr, int group, int sfb)
{
   MLO_Hcb        c;
   MLO_Boolean    noise_flag = MLO_FALSE;

   MLO_ASSERT (sec_ptr != 0);
   MLO_ASSERT (group >= 0);
   MLO_ASSERT (group < MLO_ARRAY_SIZE (sec_ptr->sect_cb));
   MLO_ASSERT (sfb >= 0);
   MLO_ASSERT (sfb < MLO_ARRAY_SIZE (sec_ptr->sect_cb [0]));

   c = sec_ptr->sfb_cb [group] [sfb];
   if (c == MLO_HCB_NOISE_HCB)
   {
      noise_flag = MLO_TRUE;
   }

   return (noise_flag);
}
