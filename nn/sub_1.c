#include "ccas.h"
#include "nn.h"
#include "wide.h"

uint_t nn_sub_1(nn_t a, nn_src_t b, int_t m, uint_t bw)
{
   CCAS_ASSERT(a == b || !NN_OVERLAP(a, m, b, m));
   CCAS_ASSERT(m >= 0);

   int_t i;

   for (i = 0; i < m && bw != 0; i++)
      bw = widesub(&a[i], b[i], bw);

   if (a != b)
      for ( ; i < m; i++)
         a[i] = b[i];

   return bw;
}

