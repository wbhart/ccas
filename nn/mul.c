#include "ccas.h"
#include "nn.h"

void nn_mul(nn_t p, nn_src_t a, int_t m, nn_src_t b, int_t n)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n >= 0);
   CCAS_ASSERT(!NN_OVERLAP(p, m + n, a, m));
   CCAS_ASSERT(!NN_OVERLAP(p, m + n, b, n));

   int_t r;
   nn_t t;
   TMP_INIT;
   
   if (n <= MUL_KARATSUBA_THRESHOLD)
   {
      nn_mul_classical(p, a, m, b, n);

      return;
   } 
   
   if (n >= (m + 1)/2)
   {
      if (n <= MUL_FFT_THRESHOLD)
         nn_mul_karatsuba(p, a, m, b, n);
      else /* too big for Karatsuba */
         nn_mul_fft(p, a, m, b, n);

      return;
   }

   r = m;

   while (r > n)
      r -= n;

   nn_mul(p, b, n, a, r);

   TMP_START;
   t = (nn_t) TMP_ALLOC((n + 1)*sizeof(uint_t));

   while (m > r)
   {
      nn_copyi(t, p + r, n); /* temporarily save top n + 1 limbs */
      nn_mul_m(p + r, a + r, b, n);
      nn_add(p + r, p + r, 2*n, t, n);
      r += n;
   }

   TMP_END;
}

