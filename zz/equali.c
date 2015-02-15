#include "ccas.h"
#include "zz.h"

int zz_equali(zz_srcptr a, int_t c)
{
   if (c == 0)
      return a->size == 0;
   else if (c > 0)
      return a->size == 1 && a->n[0] == c;
   else
      return a->size == -1 && a->n[0] == -c;
}

