#include "ccas.h"
#include "zz.h"

void zz_gcd(zz_ptr g, zz_srcptr a, zz_srcptr b)
{
   int_t asize = CCAS_ABS(a->size);
   int_t bsize = CCAS_ABS(b->size);
   int_t size;
   nn_t ta, tb;
   TMP_INIT;

   if (asize == 0)
      zz_set(g, b);
   else if (bsize == 0)
      zz_set(g, a);
   else
   {
      TMP_START;

      zz_order(a, asize, b, bsize);

      ta = TMP_ALLOC(asize*sizeof(uint_t));
      tb = TMP_ALLOC(bsize*sizeof(uint_t));

      nn_copyi(ta, a->n, asize);
      nn_copyi(tb, b->n, bsize);

      zz_fit(g, bsize);
      size = nn_gcd(g->n, ta, asize, tb, bsize);
      
      g->size = (a->size & b->size) < 0 ? -size : size;

      TMP_END;
   }
}

