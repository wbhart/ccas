#include "ccas.h"
#include "nn.h"

void nn_copyi(nn_t a, nn_src_t b, int_t m)
{
   CCAS_ASSERT(a <= b || !NN_OVERLAP(a, m, b, m));
   CCAS_ASSERT(m >= 0);

   for (int_t i = 0; i < m; i++)
      a[i] = b[i];
}

