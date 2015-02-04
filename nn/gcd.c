#include "ccas.h"
#include "nn.h"

int_t nn_gcd(nn_t g, nn_t a, int_t m, nn_t b, int_t n)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n > 0);

   int_t sgn = -1;
   nn_t q;
   TMP_INIT;

   TMP_START;
   q = (nn_t) TMP_ALLOC((m - n + 1)*sizeof(uint_t));
   nn_t r = (nn_t) TMP_ALLOC(n*sizeof(uint_t));

   nn_copyi(g, b, n);
   nn_divrem(q, r, a, m, g, n);
   
   m = n;
   n = nn_normalise(r, n);

   nn_swap(r, g);

   /* a = bq + r; a = b; b = r; */
   while (n)
   {
      nn_divrem(q, r, r, m, g, n);
      m = n;
      n = nn_normalise(r, n);
      nn_swap(r, g);
      sgn = -sgn;
   }

   /* write out GCD */
   if (sgn > 0)
      nn_copyi(g, r, m);
   
   TMP_END;

   return m;
}

