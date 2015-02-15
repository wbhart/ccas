#include "ccas.h"
#include "zz.h"

void zz_neg(zz_ptr r, zz_srcptr a)
{
   if (r != a)
   {
      int_t usize = CCAS_ABS(a->size);
      zz_fit(r, usize);
      
      nn_copyi(r->n, a->n, usize);
   }

   r->size = -a->size;
}

