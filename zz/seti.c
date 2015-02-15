#include "ccas.h"
#include "zz.h"

void zz_seti(zz_ptr r, int_t c)
{
   if (c != 0)
   {
      zz_fit(r, 1);
      r->n[0] = CCAS_ABS(c);
      r->size = c > 0 ? 1 : -1;
   } else
      r->size = 0;
}

