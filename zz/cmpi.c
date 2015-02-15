#include "ccas.h"
#include "zz.h"

int zz_cmpi(zz_srcptr a, int_t c)
{
   int_t asize = a->size;
   int_t csize = c == 0 ? 0 : (c > 0 ? 1 : -1);
   uint_t uc = (uint_t) CCAS_ABS(c);
   int sgn = asize > 0 ? 1 : -1;

   if (asize > csize)
      return 1;
   else if (asize < csize)
      return -1;
   else if (asize == 0)
      return 0;

   if (a->n[0] == uc)
      return 0;
   
   return a->n[0] > uc ? sgn : -sgn;
}

