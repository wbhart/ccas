#include "ccas.h"
#include "nn.h"

int_t xgcd(nn_t g, nn_t s, int_t * sn, nn_src_t a, int_t m,
                                       nn_src_t b, int_t n)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n > 0);
   CCAS_ASSERT(!NN_OVERLAP(g, n, s, n));

   int sgn = -1;
   int_t un = 0, qn, pn;
   uint_t cy;
   TMP_INIT;

   TMP_START;
   nn_t u = (nn_t) TMP_ALLOC(n*sizeof(uint_t));
   nn_t q = (nn_t) TMP_ALLOC((m - n + 1)*sizeof(uint_t));
   nn_t r = (nn_t) TMP_ALLOC(n*sizeof(uint_t));
   nn_t p = (nn_t) TMP_ALLOC((n + 1)*sizeof(uint_t));

   nn_copyi(g, b, n);
   nn_divrem(q, r, a, m, g, n);
   m = n;
   n = nn_normalise(r, n);

   nn_swap(g, r);

   /* set s = 0, u = 1 */
   nn_zero(s, n);
   nn_zero(u, n);
   u[0] = 1;
   (*sn) = 0;

   while (n != 0)
   {
      nn_divrem(q, r, r, m, g, n);
      qn = nn_normalise(q, m - n + 1);
      m = n;
      n = nn_normalise(r, n);
      nn_swap(g, r);
 
      /* 
         set s, u = u, s - q*u
         sign of u always opposite of that of s
         (which alternates), thus s and u can be
         stored as monotonically increasing
         nonnegative values 
      */
      if (un >= qn)
         nn_mul(p, u, un, q, qn);
      else
         nn_mul(p, q, qn, u, un);

      pn = nn_normalise(p, qn + un);

      if (nn_add(s, p, pn, s, *sn))
         s[pn++] = 1;

      nn_swap(s, u);
      (*sn) = un;
      un = nn_normalise(u, pn);

      sgn = -sgn;
   }

   /* deal with swapping and sign of s */
   if (sgn < 0)
      (*sn) = -(*sn);
   else
   {
      nn_copyi(r, g, m);
      nn_copyi(u, s, *sn);
   }

   TMP_END;

   return m;
}

