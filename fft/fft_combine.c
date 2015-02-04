#include "ccas.h"
#include "fft.h"

void fft_combine(nn_t a, int_t m, nn_t * fft, int_t n, 
            int_t coeff_limbs, int_t limbs)
{
   CCAS_ASSERT(coeff_limbs >= 0);
   CCAS_ASSERT(limbs >= coeff_limbs);
   CCAS_ASSERT(m >= 0);
   CCAS_ASSERT(n >= 0);
   
   int_t off, i;
   
   nn_zero(a, m);

   for (off = 0, i = 0; i < n && off + limbs + 1 <= m; i++, off += coeff_limbs) 
      nn_add(a + off, a + off, limbs + 1, fft[i], limbs); 

   for ( ; off < m && i < n; i++, off += coeff_limbs)
      nn_add(a + off, a + off, m - off, fft[i], CCAS_MIN(m - off, limbs));
}

