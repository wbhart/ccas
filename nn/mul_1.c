#include "ccas.h"
#include "nn.h"
#include "wide.h"

uint_t nn_mul_1(nn_t a, nn_src_t b, int_t m, uint_t c)
{
   CCAS_ASSERT(a == b || !NN_OVERLAP(a, m, b, m));
   CCAS_ASSERT(m >= 0);
   
   uint_t cy = 0;

   for (int_t i = 0; i < m; i++)
      cy = widemul(&a[i], b[i], c, cy);

   return cy;
}

