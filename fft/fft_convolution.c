#include "ccas.h"
#include "fft.h"

void fft_convolution(nn_t * ii, nn_t * jj, int_t n, int_t limbs)
{
   CCAS_ASSERT(n >= 0);
   CCAS_ASSERT(limbs >= 0);
   CCAS_ASSERT(POWEROF2(n));
   CCAS_ASSERT(limbs*INT_BITS % n == 0);

   int_t depth = BITCOUNT(n) - 1, j;
   int_t w = (limbs*INT_BITS)/n;
   
   fft_radix2(ii, n, w);
   
   if (ii != jj)
      fft_radix2(jj, n, w);

   for (j = 0; j < 2*n; j++)
   {
      fft_normalise(ii[j], limbs);
      if (ii != jj)
         fft_normalise(jj[j], limbs);
         
      fft_mulmod(ii[j], ii[j], jj[j], limbs);
   }

   ifft_radix2(ii, n, w);

   for (j = 0; j < 2*n; j++)
   {
      fft_div_2exp(ii[j], ii[j], limbs, depth + 1);
      fft_normalise(ii[j], limbs);
   }
}

