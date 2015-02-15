#include "ccas.h"
#include "zz.h"

void zz_fit(zz_ptr r, int_t m)
{
   CCAS_ASSERT(m >= 0);

   if (r->alloc < m)
   {
      r->n = (nn_t) realloc(r->n, m*sizeof(uint_t));
      r->alloc = m;
   }
}

