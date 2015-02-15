#include "ccas.h"
#include "zz.h"

void zz_muli(zz_ptr r, zz_srcptr a, int_t c)
{
   int_t usize = CCAS_ABS(a->size);
   
   if (c == 0 || usize == 0)
      r->size = 0;
   else
   {
      zz_fit(r, usize + 1);

      r->n[usize] = nn_mul_1(r->n, a->n, usize, c);
      usize += (r->n[usize] != 0);

      r->size = a->size >= 0 ? usize : -usize;
   }
}

