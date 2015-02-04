#include "ccas.h"
#include "nn.h"

int_t nn_normalise(nn_src_t a, int_t m)
{
   CCAS_ASSERT(m >= 0);

   while (m != 0 && a[m - 1] == 0)
      m--;

   return m;
}

