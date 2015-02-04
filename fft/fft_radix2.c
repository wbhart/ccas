#include "ccas.h"
#include "fft.h"

void fft_radix2(nn_t * ii, int_t n, int_t w)
{
   CCAS_ASSERT(n >= 0);
   CCAS_ASSERT(w >= 0);
   CCAS_ASSERT(w*n % INT_BITS == 0);

   int_t limbs = (w*n)/INT_BITS;
   TMP_INIT;

   TMP_START;
   
   nn_t t1 = TMP_ALLOC((limbs + 1)*sizeof(uint_t));
   nn_t t2 = TMP_ALLOC((limbs + 1)*sizeof(uint_t));

   for (int_t i = 0; i < n; i++) 
   {   
      fft_butterfly(t1, t2, ii[i], ii[n + i], limbs, i*w);
   
      nn_copyi(ii[i],     t1, limbs + 1);
      nn_copyi(ii[n + i], t2, limbs + 1);
   }

   TMP_END;

   if (n == 1)
      return;

   fft_radix2(ii,     n/2, 2*w);
   fft_radix2(ii + n, n/2, 2*w);
}

