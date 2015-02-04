#include "ccas.h"
#include "nn.h"

void nn_copyd(nn_t a, nn_src_t b, int_t m)
{
   CCAS_ASSERT(a >= b || !NN_OVERLAP(a, m, b, m));   
   CCAS_ASSERT(m >= 0);
 
   for (m--; m >= 0; m--)
      a[m] = b[m];
}

