#include "ccas.h"
#include "nn.h"

int nn_cmp(nn_t a, nn_src_t b, int_t m)
{
   CCAS_ASSERT(m >= 0);

   for (m--; m >= 0; m--) 
      if (a[m] != b[m]) 
         return a[m] > b[m] ? 1 : -1;

   return 0;
}

