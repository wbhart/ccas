#include "ccas.h"
#include "zz.h"

void zz_init(zz_ptr r)
{
   r->n = NULL;
   r->size = 0;
   r->alloc = 0;
}

