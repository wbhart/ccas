#include "ccas.h"
#include "nn.h"
#include "wide.h"

uint_t nn_divrem_1_pi1(nn_t q, uint_t cy, nn_src_t a, int_t m, 
                                                   uint_t d, uint_t dinv)
{
   CCAS_ASSERT(q == a || !NN_OVERLAP(q, m, a, m));
   CCAS_ASSERT(cy < d);
   CCAS_ASSERT(m >= 0);
   CCAS_ASSERT(NORMALISED(d));

   for (m--; m >= 0; m--)
      cy = widedivrem_pi1(&q[m], cy, a[m], d, dinv);

   return cy;
}

