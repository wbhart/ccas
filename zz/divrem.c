#include "ccas.h"
#include "zz.h"

void zz_divrem(zz_ptr q, zz_ptr r, zz_srcptr a, zz_srcptr b)
{
   int_t asize = CCAS_ABS(a->size);
   int_t bsize = CCAS_ABS(b->size);
   int_t rsize = bsize;
   int_t qsize = asize - bsize + 1;

   if (asize < bsize)
      q->size = 0;
   else
   {
      zz_fit(q, qsize);
      zz_fit(r, rsize);

      nn_divrem(q->n, r->n, a->n, asize, b->n, bsize);
         
      qsize -= q->n[qsize - 1] == 0;
      rsize = nn_normalise(r->n, rsize);

      q->size = (a->size ^ b->size) >= 0 ? qsize : -qsize;
      r->size = a->size >= 0 ? rsize : -rsize;

      if (q->size < 0 && r->size != 0)
      {
         zz_subi(q, q, 1);
         zz_add(r, r, b);
      }
   }
}

