#include "ccas.h"
#include "nn.h"

int_t nn_gcd_euclidean(nn_t g, nn_t a, int_t m, nn_t b, int_t n)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n > 0);

   TMP_INIT;

   TMP_START;
   nn_t q = (nn_t) TMP_ALLOC((m - n + 1)*sizeof(uint_t));
   nn_t r1 = (nn_t) TMP_ALLOC(m*sizeof(uint_t));
   nn_t r2 = (nn_t) TMP_ALLOC(n*sizeof(uint_t));

   nn_copyi(r1, a, m);
   nn_copyi(r2, b, n);
   
   /* a = bq + r; a = b; b = r; */
   while (n != 0)
   {
      nn_divrem(q, r1, r1, m, r2, n);
      m = n;
      n = nn_normalise(r1, n);
      nn_swap(r1, r2);
   }

   /* write out GCD */
   nn_copyi(g, r1, m);
   
   TMP_END;

   return m;
}

