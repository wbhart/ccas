#include "ccas.h"
#include "nn.h"
#include "wide.h"

uint_t nn_neg(nn_t a, nn_src_t b, int_t m)
{
   CCAS_ASSERT(a == b || !NN_OVERLAP(a, m, b, m));
   CCAS_ASSERT(m >= 0);

   int_t i;
   uint_t cy = 1;

   /* -b = ~b + 1 */
   for (i = 0; i < m && cy != 0; i++)
      cy = cmc(&a[i], b[i], cy);

   /* no more carries, just complement */
   for ( ; i < m; i++)
      a[i] = ~b[i];

   return 1 - cy;
}

