#include "ccas.h"
#include "nn.h"

int_t xgcd_euclidean(nn_t g, nn_t s, int_t * sn, nn_src_t a, int_t m,
                                                 nn_src_t b, int_t n)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n > 0);
   CCAS_ASSERT(!NN_OVERLAP(g, n, s, n));

   int sgn = -1;
   int_t un, vn, qn, pn;
   TMP_INIT;

   TMP_START;
   nn_t u = (nn_t) TMP_ALLOC(n*sizeof(uint_t));
   nn_t v = (nn_t) TMP_ALLOC(n*sizeof(uint_t));
   nn_t q = (nn_t) TMP_ALLOC((m - n + 1)*sizeof(uint_t));
   nn_t r1 = (nn_t) TMP_ALLOC(m*sizeof(uint_t));
   nn_t r2 = (nn_t) TMP_ALLOC(n*sizeof(uint_t));
   nn_t p = (nn_t) TMP_ALLOC((n + 1)*sizeof(uint_t));

   nn_copyi(r2, b, n);

   /* do first division */
   nn_divrem(q, r1, a, m, r2, n);
   m = n;
   n = nn_normalise(r1, n);
   nn_swap(r1, r2);
   
   /* set u = 0, v = 1 */
   nn_zero(u, n);
   nn_zero(v, n);
   v[0] = 1;
   un = 0;
   vn = 1;

   while (n != 0)
   {
      /* r1, r2 = r2, r1 - q*r2 */
      nn_divrem(q, r1, r1, m, r2, n);
      qn = nn_normalise(q, m - n + 1);
      m = n;
      n = nn_normalise(r1, n);
      nn_swap(r1, r2);
 
      /* 
         set u, v = v, u - q*v
         sign of u always opposite of that of v
         (which alternates), thus u and v can be
         stored as monotonically increasing
         nonnegative values
      */
      if (vn >= qn)
         nn_mul(p, v, vn, q, qn);
      else
         nn_mul(p, q, qn, v, vn);

      pn = nn_normalise(p, qn + vn);

      if (nn_add(u, p, pn, u, un))
         u[pn++] = 1;

      nn_swap(u, v);
      un = vn;
      vn = nn_normalise(v, pn);

      sgn = -sgn;
   }

   (*sn) = sgn < 0 ? un : -un;
      
   nn_copyi(g, r1, m);
   nn_copyi(s, u, un);

   TMP_END;

   return m;
}

