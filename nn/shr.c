#include "ccas.h"
#include "nn.h"
#include "wide.h"

uint_t nn_shr(nn_t a, nn_src_t b, int_t m, int_t bits)
{
   CCAS_ASSERT(a == b || !NN_OVERLAP(a, m, b, m));
   CCAS_ASSERT(bits >= 0 && bits < INT_BITS);
   CCAS_ASSERT(m >= 0);

   int_t cy = 0;
   
   for (m--; m >= 0; m--) 
      cy = wideshr(&a[m], b[m], bits, cy);

   return cy;
}

