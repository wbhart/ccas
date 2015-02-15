#include "ccas.h"
#include "zz.h"

void zz_mul_2exp(zz_ptr r, zz_srcptr a, int_t s)
{
   CCAS_ASSERT(s >= 0);

   if (a->size == 0)
      r->size = 0;
   else
   {
      int_t bits = s & (INT_BITS - 1);
      int_t words = s / INT_BITS;
      int_t usize = CCAS_ABS(a->size);
      int_t rsize = usize + words;
   
      zz_fit(r, rsize + (bits != 0));

      if (bits == 0)
         nn_copyd(r->n + words, a->n, usize);
      else
      {
         uint_t cy = r->n[rsize] = nn_shl(r->n + words, a->n, usize, bits);
         rsize += (cy != 0);  
      }

      nn_zero(r->n, words);
      r->size = a->size >= 0 ? rsize : -rsize;
   }
}

