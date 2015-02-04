#include "ccas.h"
#include "nn.h"
#include "wide.h"

uint_t nn_add_1(nn_t a, nn_src_t b, int_t m, uint_t cy)
{
   CCAS_ASSERT(a == b || !NN_OVERLAP(a, m, b, m));
   CCAS_ASSERT(m >= 0);

   int_t i;

   for (i = 0; i < m && cy != 0; i++)
      cy = wideadd(&a[i], b[i], cy);

   if (a != b)
      for ( ; i < m; i++)
         a[i] = b[i];

   return cy;
}

