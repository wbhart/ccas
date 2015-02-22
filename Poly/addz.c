#include "ccas.h"
#include "Poly.h"

void poly_addz(Poly_t a, Poly_t b, zz_t c)
{
   if (b->length == 0)
      poly_setz(a, c);
   else
   {
      void (* addz)(void *, void *, zz_t) = a->parent->base->addz;

      poly_set(a, b);

      addz(a->coeffs, a->coeffs, c);

      poly_normalise(a);
   }
}

