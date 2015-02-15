#include "ccas.h"
#include "zz.h"

int zz_equal(zz_srcptr a, zz_srcptr b)
{
   if (a == b)
      return 1;

   int_t un;

   if (a->size != b->size)
      return 0;

   return nn_cmp(a->n, b->n, CCAS_ABS(a->size)) == 0;
}

