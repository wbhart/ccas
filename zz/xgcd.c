#include "ccas.h"
#include "zz.h"

void zz_xgcd(zz_ptr g, zz_ptr s, zz_ptr t, zz_srcptr a, zz_srcptr b)
{
   int_t m = CCAS_ABS(a->size);
   int_t n = CCAS_ABS(b->size);

   /* ensure m >= n */
   if (m < n)
   {
      zz_order(a, m, b, n);
      zz_ptrswap(s, t);
   }

   /* ensure n > 0 */
   if (n == 0)
   {
      zz_set(g, a);
      zz_seti(s, 1);
      zz_seti(t, 0);

      return;
   }

   /* compute gcd and one cofactor */
   zz_fit(s, n);
   zz_fit(g, n);

   g->size = nn_xgcd(g->n, s->n, &(s->size), a->n, m, b->n, n);

   /* gcd is negative if a and b are */
   if (a->size < 0 && b->size < 0)
   {
      zz_neg(g, g);
      zz_neg(s, s);
   }

   /* compute second cofactor, t = (g - as)/b */
   zz_t p;

   zz_init(p);

   zz_mul(p, a, s);
   zz_sub(p, g, s);
   zz_div(t, p, b);

   zz_clear(p);

   return;
}

