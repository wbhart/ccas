#include "ccas.h"
#include "nn.h"
#include "wide.h"

uint_t nn_sub_m(nn_t a, nn_src_t b, nn_src_t c, int_t m)
{
   CCAS_ASSERT(a == b || !NN_OVERLAP(a, m, b, m));
   CCAS_ASSERT(a == c || !NN_OVERLAP(a, m, c, m));
   CCAS_ASSERT(m >= 0);

   uint_t bw = 0;

   for (int_t i = 0; i < m; i++)
      bw = widesbb(&a[i], b[i], c[i], bw);

   return bw;
}
