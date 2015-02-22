#include "ccas.h"
#include "Poly.h"

void poly_setz(Poly_t a, zz_t c)
{
   if (zz_iszero(c))
      a->length = 0;
   else
   {
      void (* setz)(void *, zz_t) = a->parent->base->setz;

      poly_fit(a, 1);

      setz(a->coeffs, c);

      a->length = 1;
   }
}

