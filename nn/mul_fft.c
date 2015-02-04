#include "ccas.h"
#include "fft.h"

void nn_mul_fft(nn_t r, nn_src_t a1, int_t m1, nn_src_t a2, int_t m2)
{
   CCAS_ASSERT(m1 >= 0);
   CCAS_ASSERT(m2 >= 0);

   /* minimum n, w pair */
   int_t n = INT_BITS;
   int_t w = 2;

   /* total output size */
   int_t m = m1 + m2;

   /* find n, w pair such that result takes no more than 2*n + 1 coeffs */
   while ((2*n + 1)*(n*w/(2*INT_BITS) - 1) < m)
   {
      n = w == 1 ? n : 2*n;
      w = w == 1 ? 2 : 1;
   }

   int_t limbs = n*w / INT_BITS;

   /* allocate arrays of fft coefficients */
   nn_t * ii = (nn_t *) malloc(2*n*sizeof(nn_t *));
   nn_t * jj = a1 == a2 ? ii : (nn_t *) malloc(2*n*sizeof(nn_t *));
   nn_t d1 = (nn_t) malloc((limbs + 1)*2*n*sizeof(uint_t));
   nn_t d2 = a1 == a2 ? d1 : (nn_t) malloc((limbs + 1)*2*n*sizeof(uint_t));
   
   for (int_t i = 0; i < 2*n; i++)
   {
      ii[i] = d1 + i*(limbs + 1);
      if (ii != jj)
         jj[i] = d2 + i*(limbs + 1);
   }

   /* split integers into fft arrays */
   int_t coeff_limbs = n*w/(2*INT_BITS) - 1;

   fft_split(ii, 2*n, a1, m1, coeff_limbs, limbs);
   if (ii != jj)
      fft_split(jj, 2*n, a2, m2, coeff_limbs, limbs);

   /* do fft convolution */
   fft_convolution(ii, jj, n, limbs);

   /* combine fft coefficients into result integer r */
   fft_combine(r, m, ii, 2*n, coeff_limbs, limbs);

   /* clean up */
   if (ii != jj)
   {
      free(d2);
      free(jj);
   }
   free(ii);
   free(d1);
}

