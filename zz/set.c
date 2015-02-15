#include "ccas.h"
#include "zz.h"

void zz_set(zz_ptr r, zz_srcptr a)
{
   if (r != a)
   {
      int_t un = CCAS_ABS(a->size);

      zz_fit(r, un);

      nn_copyi(r->n, a->n, un);

      r->size = a->size;
   }
}

