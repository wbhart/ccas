#include "ccas.h"
#include "zz.h"

void zz_div_2exp(zz_ptr r, zz_srcptr a, int_t s)
{
   CCAS_ASSERT(s >= 0);

   int_t bits = s & (INT_BITS - 1);
   int_t words = s / INT_BITS;
   int_t usize = CCAS_ABS(a->size);
   int_t rsize = usize - words;
   
   if (rsize <= 0)
      r->size = 0;
   else
   {
      zz_fit(r, rsize);

      if (bits == 0)
         nn_copyi(r->n, a->n + words, rsize);
      else
      {
         nn_shr(r->n, a->n + words, rsize, bits);
         rsize -= (r->n[rsize - 1] == 0);  
      }

      r->size = a->size >= 0 ? rsize : -rsize;
   }
}

