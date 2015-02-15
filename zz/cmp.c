#include "ccas.h"
#include "zz.h"

int zz_cmp(zz_srcptr a, zz_srcptr b)
{
   int_t asize = a->size;
   int_t bsize = b->size;
   int sgn;

   if (asize > bsize)
      return 1;
   else if (asize < bsize)
      return -1;

   sgn = nn_cmp(a->n, b->n, CCAS_ABS(asize));
   return asize < 0 ? -sgn : sgn;
}

