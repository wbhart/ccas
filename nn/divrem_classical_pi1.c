#include "ccas.h"
#include "nn.h"
#include "wide.h"

void nn_divrem_classical_pi1(nn_t q, uint_t cy, nn_t a, int_t m, 
                                          nn_src_t d, int_t n, uint_t pi1)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n > 0);
   CCAS_ASSERT(!NN_OVERLAP(q, m - n + 1, a, m));
   CCAS_ASSERT(!NN_OVERLAP(q, m - n + 1, d, n));
   CCAS_ASSERT(!NN_OVERLAP(a, m, d, n));
   CCAS_ASSERT(NORMALISED(d[n - 1]));
   CCAS_ASSERT(cy < d[n - 1] 
          || (cy == d[n - 1] && nn_cmp(a + m - n + 1, d, n - 1) < 0));
   
   int_t i = m - 1, j = m - n;
   uint_t d1 = d[n - 1], hi = 0;
   
   for ( ; i >= n - 1; i--, j--)
   {
      /* special case, cy == d1 could cause overflow */
      q[j] = cy == d1 ? ~(uint_t) 0 : widedivappr_pi1(cy, a[i], d1, pi1);

      /* a -= d*q1 */
      cy -= nn_submul_1(a + j, d, n, q[j]);
      
      /* correct if remainder is not less than d */
      while (cy > 0 || (cy == 0 && nn_cmp(a + j, d, n) >= 0))
      {
         q[j]++;
         cy -= nn_sub_m(a + j, a + j, d, n);
      }
      
      /* fetch next word */
      cy = a[i];
   }
}

