#include "ccas.h"
#include "nn.h"
#include "wide.h"

uint_t nn_shl(nn_t a, nn_src_t b, int_t m, int_t bits)
{
   CCAS_ASSERT(a == b || !NN_OVERLAP(a, m, b, m));
   CCAS_ASSERT(bits >= 0 && bits < INT_BITS);
   CCAS_ASSERT(m >= 0);

   int_t cy = 0;
   
   for (int_t i = 0; i < m; i++)
      cy = wideshl(&a[i], b[i], bits, cy);

   return cy;
}

