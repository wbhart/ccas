#include "ccas.h"
#include "zz.h"

void zz_clear(zz_ptr r)
{
   free(r->n);
}

