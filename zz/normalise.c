#include "ccas.h"
#include "zz.h"

void zz_normalise(zz_ptr r)
{
   if (r->size < 0)
      r->size = -nn_normalise(r->n, -r->size);
   else
      r->size = nn_normalise(r->n, r->size);
}

