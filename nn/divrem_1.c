#include "ccas.h"
#include "nn.h"
#include "wide.h"

uint_t nn_divrem_1(nn_t q, uint_t cy, nn_src_t a, int_t m, uint_t d)
{
   CCAS_ASSERT(q == a || !NN_OVERLAP(q, m, a, m));
   CCAS_ASSERT(d != 0);
   CCAS_ASSERT(cy < d);

   for (m--; m >= 0; m--)
      cy = widedivrem(&q[m], cy, a[m], d);

   return cy;
}

