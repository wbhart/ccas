#include "ccas.h"
#include "zz.h"

int zz_cmpabs(zz_srcptr a, zz_srcptr b)
{
   int_t asize = CCAS_ABS(a->size);
   int_t bsize = CCAS_ABS(b->size);
   
   if (asize > bsize)
      return 1;
   else if (asize < bsize)
      return -1;

   return nn_cmp(a->n, b->n, asize);
}

