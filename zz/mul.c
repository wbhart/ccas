#include "ccas.h"
#include "zz.h"

void zz_mul(zz_ptr r, zz_srcptr a, zz_srcptr b)
{
   int_t asize = CCAS_ABS(a->size);
   int_t bsize = CCAS_ABS(b->size);
   int_t rsize = asize + bsize;
   
   if (asize == 0 || bsize == 0)
      rsize = 0;
   else
   {
      zz_fit(r, rsize);

      zz_order(a, asize, b, bsize);

      nn_mul(r->n, a->n, asize, b->n, bsize);
      rsize -= (r->n[rsize - 1] == 0);

      r->size = (a->size ^ b->size) < 0 ? -rsize : rsize;
   }
}

