#include "ccas.h"
#include "nn.h"

void nn_randbits(nn_t a, int_t m, rand_t state, int_t bits)
{
   CCAS_ASSERT(bits >= 0);
   CCAS_ASSERT(bits <= m*INT_BITS);

   int_t numwords = bits / INT_BITS;
   int_t numbits = bits % INT_BITS;
   int_t i;

   for (i = 0; i < numwords; i++)
      a[i] = randword(state);

   if (numbits != 0)
      a[i++] = randbits(state, numbits);

   for ( ; i < m; i++)
      a[i] = 0;
}

