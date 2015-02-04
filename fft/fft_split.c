#include "ccas.h"
#include "fft.h"

void fft_split(nn_t * fft, int_t n, nn_src_t a, 
                          int_t m, int_t coeff_limbs, int_t limbs)
{
   CCAS_ASSERT(coeff_limbs >= 0);
   CCAS_ASSERT(limbs >= coeff_limbs);
   CCAS_ASSERT(m >= 0);
   CCAS_ASSERT(n >= 0);

   int_t i, off;
   
   for (off = 0, i = 0; off + coeff_limbs <= m; off += coeff_limbs, i++)
   {
      nn_zero(fft[i] + coeff_limbs, limbs - coeff_limbs + 1);
      nn_copyi(fft[i], a + off, coeff_limbs);
   }
   
   /* partial coefficient of length m - off remains */
   if (m > off)
   {
      nn_zero(fft[i] + m - off, limbs - m + off + 1);
      nn_copyi(fft[i++], a + off, m - off);
   }

   /* zero remaining fft coefficients */
   for ( ; i < n; i++)
      nn_zero(fft[i], limbs + 1);
}

