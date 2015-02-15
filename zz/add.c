#include "ccas.h"
#include "zz.h"

void zz_add(zz_ptr r, zz_srcptr a, zz_srcptr b)
{
   int_t asize = CCAS_ABS(a->size);
   int_t bsize = CCAS_ABS(b->size);
   int_t rsize;

   zz_order(a, asize, b, bsize);
   
   zz_fit(r, asize + 1);

   if ((a->size ^ b->size) < 0)
   {
      uint_t bw = nn_sub(r->n, a->n, asize, b->n, bsize);
      rsize = a->size;
      
      if (bw != 0)
      {
         nn_neg(r->n, r->n, asize);
         rsize = -rsize;
      }
   } else
   {
      r->n[asize] = nn_add(r->n, a->n, asize, b->n, bsize);
      rsize = asize + 1;
      if (a->size < 0)
         rsize = -rsize;
   }

   r->size = rsize;
   zz_normalise(r);
}

