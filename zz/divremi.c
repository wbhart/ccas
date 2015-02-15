#include "ccas.h"
#include "zz.h"
#include "wide.h"

long zz_divremi(zz_ptr q, zz_srcptr a, int_t b)
{
   CCAS_ASSERT(b != 0);

   int_t asize = CCAS_ABS(a->size);
   int_t r, qsize = asize;
   zz_t t;
   TMP_INIT;

   if (asize == 0)
   {
      q->size = 0;
      r = 0;
   } else
   {
      uint_t d = (uint_t) CCAS_ABS(b);
      int_t norm = INT_BITS - BITCOUNT(d);
      
      d <<= norm;

      uint_t pi1 = preinvert1(d);

      TMP_START;
      nn_t t = (nn_t) TMP_ALLOC(CCAS_ABS(a->size));

      uint_t cy = nn_shl(t, a->n, asize, norm);

      zz_fit(q, qsize);
   
      r = (nn_divrem_1_pi1(q->n, cy, t, asize, d, pi1) >> norm);
         
      qsize -= q->n[qsize - 1] == 0;
      
      q->size = (a->size ^ b) >= 0 ? qsize : -qsize;
      r = a->size >= 0 ? r : -r;

      if (q->size < 0 && r != 0)
      {
         zz_subi(q, q, 1);
         r += b;
      }

      TMP_END;
   }

   return r;
}

