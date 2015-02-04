#include "ccas.h"
#include "fft.h"

void fft_mul_2exp(nn_t t, nn_src_t i1, int_t limbs, int_t i)
{
   CCAS_ASSERT(i < INT_BITS);
   CCAS_ASSERT(i >= 0);
   CCAS_ASSERT(limbs >= 0);

   int_t cy1, cy2;
   
   /* when i = 0 a shift of INT_BITS would not be defined, so subtract 1 */
   cy1 = ((int_t) i1[limbs] >> (INT_BITS - i - (i == 0))); 
   nn_shl(t, i1, limbs + 1, i);
   cy2 = t[limbs];
   t[limbs] = 0;
   nn_sub_1(t, t, limbs + 1, cy2);
   fft_add_1(t + 1, limbs - 1, -cy1);
}

