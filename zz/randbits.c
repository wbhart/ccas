#include "ccas.h"
#include "zz.h"

void zz_randbits(zz_ptr a, rand_t state, int_t bits)
{
   int_t ubits = CCAS_ABS(bits);
   int_t size = (ubits + INT_BITS - 1)/INT_BITS;

   zz_fit(a, size);

   nn_randbits(a->n, state, ubits);

   size = nn_normalise(a->n, size);
   a->size = (bits < 0 && randint(state, 2) == 0) ? -size : size;
}

