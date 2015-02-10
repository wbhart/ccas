#include "ccas.h"
#include "nn.h"

int nn_ngcd_sdiv_cmp(nn_src_t a, int_t m, nn_src_t b, int_t n, int_t s)
{
   CCAS_ASSERT(s > 0);
   CCAS_ASSERT(n > s);
   CCAS_ASSERT(m >= n);
   TMP_INIT;

   if (m > n + 1)
      return 1;

   if (m > n && a[m - 1] > 1)
      return 1;

   TMP_START;

   nn_t t = (nn_t) TMP_ALLOC(m*sizeof(uint_t));
      
   uint_t cy = nn_sub(t, a, m, b, n);
   if (cy != 0)
      nn_neg(t, t, n);

   n = nn_normalise(t, m);

   TMP_END;

   return n > s;
}

