#include "ccas.h"
#include "nn.h"

void nn_ngcd_sdiv(nn_t q, nn_t r, nn_src_t a, int_t m, 
                                  nn_src_t b, int_t n, int_t s)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n > 0);
   CCAS_ASSERT(n >= s);
   CCAS_ASSERT(!NN_OVERLAP(q, m - n + 1, r, n));

   uint_t cy = 0;

   nn_divrem(q, r, a, m, b, n);
   
   int_t rn = nn_normalise(r, n);
   int_t qn = nn_normalise(q, m - n + 1);

   if (rn < s)
   {
      nn_sub_1(q, q, qn, 1);
      cy = nn_add(r, b, n, r, rn);
   }

   if (m > n)
      r[n] = cy;
}
