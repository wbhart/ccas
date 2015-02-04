#include "ccas.h"
#include "nn.h"

int_t nn_gcd(nn_t g, nn_t a, int_t m, nn_t b, int_t n)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n > 0);

   nn_t q;
   TMP_INIT;

   TMP_START;
   q = (nn_t) TMP_ALLOC((m - n + 1)*sizeof(uint_t));
   nn_t r0 = (nn_t) TMP_ALLOC(m*sizeof(uint_t));
   nn_t r1 = (nn_t) TMP_ALLOC(n*sizeof(uint_t));

   nn_copyi(r0, a, m);
   nn_copyi(r1, b, n);

   /* a = bq + r; a = b; b = r; */
   while (n)
   {
      nn_divrem(q, r0, r0, m, r1, n);
      nn_swap(r0, r1);
      m = n;
      n = nn_normalise(r1, n);
   }

   /* write out GCD */
   nn_copyi(g, r0, m);
   
   TMP_END;

   return m;
}

