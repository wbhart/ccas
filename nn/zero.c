#include "ccas.h"
#include "nn.h"

void nn_zero(nn_t a, int_t m)
{
   CCAS_ASSERT(m >= 0);

   for (m--; m >= 0; m--)
      a[m] = 0;
}

