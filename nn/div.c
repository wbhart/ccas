#include "ccas.h"
#include "nn.h"
#include "wide.h"

void nn_div(nn_t q, nn_src_t a, int_t m, nn_src_t d, int_t n)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n > 0);

   TMP_INIT;
   TMP_START;
   
   uint_t cy, pi1;
   int_t norm = INT_BITS - BITCOUNT(d[n - 1]);
   nn_t a2 = (nn_t) TMP_ALLOC(m*sizeof(uint_t));
   nn_t d2 = (nn_t) TMP_ALLOC(n*sizeof(uint_t));
   
   /* shift dividend and divisor left to normalise divisor */
   cy = nn_shl(a2, a, m, norm);
   nn_shl(d2, d, n, norm);
   
   /* precompute inverse */
   pi1 = preinvert1(d2[n - 1]);

   nn_div_pi1(q, cy, a2, m, d2, n, pi1);

   TMP_END;
}
