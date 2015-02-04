#include "ccas.h"
#include "fft.h"
#include "wide.h"

void fft_div_2exp(nn_t t, nn_src_t i1, int_t limbs, int_t i)
{
   CCAS_ASSERT(i < INT_BITS);
   CCAS_ASSERT(i >= 0);
   CCAS_ASSERT(limbs >= 0);

   uint_t lo;
   int_t hi;

   if (i == 0)
   {
      nn_copyi(t, i1, limbs + 1);
      return;
   }   
   hi = i1[limbs];
   lo = nn_shr(t, i1, limbs + 1, i);
   t[limbs] = (hi >> i) - widesub(&t[limbs - 1], t[limbs - 1], lo);
}

