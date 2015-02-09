#include "ccas.h"
#include "nn.h"

void nn_divrem_divconquer_pi1(nn_t q, uint_t cy, nn_t a, int_t m, 
                                             nn_src_t d, int_t n, uint_t pi1)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(m <= 2*n - 1);
   CCAS_ASSERT(n > 0);
   CCAS_ASSERT(NORMALISED(d[n - 1]));
   CCAS_ASSERT(!NN_OVERLAP(q, m - n + 1, a, m));
   CCAS_ASSERT(!NN_OVERLAP(q, m - n + 1, d, n));
   CCAS_ASSERT(!NN_OVERLAP(a, m, d, n));
   
   int_t qn = m - n + 1;
   int_t q1 = (qn + 1)/2, q2 = qn - q1;
   TMP_INIT;
   
   TMP_START;
   nn_t p = (nn_t) TMP_ALLOC((n + q1)*sizeof(uint_t));
   nn_t r = (nn_t) TMP_ALLOC(m*sizeof(uint_t));
   nn_copyi(r, a, m);
   
   /* compute top q1 words of quotient */
   nn_div_pi1(q + q2, cy, r + q2, m - q2, d, n, pi1);
   nn_mul(p, d, n, q + q2, q1);
   nn_sub_m(a + q2, a + q2, p, q1 + n);
   
   /* compute bottom q2 words of quotient */
   nn_div_pi1(q, a[m - q1], a, m - q1, d, n, pi1);
   nn_mul(p, d, n, q, q2);
   nn_sub_m(a, a, p, q2 + n);

   TMP_END;
}
