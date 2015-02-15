#include "ccas.h"
#include "zz.h"

void zz_div(zz_ptr q, zz_srcptr a, zz_srcptr b)
{
   CCAS_ASSERT(b->size != 0);

   int_t asize = CCAS_ABS(a->size);
   int_t bsize = CCAS_ABS(b->size);
   int_t rsize = bsize;
   int_t qsize = asize - bsize + 1;
   int_t qsign = (a->size ^ b->size);
   TMP_INIT;

   if (asize < bsize)
      q->size = 0;
   else
   {
      zz_fit(q, qsize);
      
      if (qsign < 0)
      {
         zz_t q2;

         zz_init(q2);
         zz_fit(q2, qsize);

         nn_div(q2->n, a->n, asize, b->n, bsize);

         /* remainder may be zero */
         if (q2->n[0]*b->n[0] == a->n[0])
         {
            TMP_START;

            nn_t p = (nn_t) TMP_ALLOC((qsize + bsize)*sizeof(uint_t));

            if (qsize >= bsize)
               nn_mul(p, q2->n, qsize, b->n, bsize);
            else
               nn_mul(p, b->n, bsize, q2->n, qsize);
            
            /* if a > q*b, there will be a borrow from sub_m */
            rsize = nn_sub_m(p, p, a->n, asize);

            TMP_END;
         } else /* remainder cannot be zero */
         {
            nn_copyi(q->n, q2->n, qsize);
            rsize = 1;
         }
      } else
         nn_div(q->n, a->n, asize, b->n, bsize);

      qsize -= q->n[qsize - 1] == 0;
      q->size = qsign >= 0 ? qsize : -qsize;
      
      if (q->size < 0 && rsize != 0)
         zz_subi(q, q, 1);
   }
}

