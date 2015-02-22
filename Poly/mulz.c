#include "ccas.h"
#include "Poly.h"

void poly_mulz(Poly_t a, Poly_t b, zz_t c)
{
   if (b->length == 0 || zz_iszero(c))
      a->length = 0;
   else if (zz_cmpi(c, 1) == 0)
      poly_set(a, b);
   else if (zz_cmpi(c, -(int_t) 1) == 0)
      poly_neg(a, b);
   else
   {
      void (* mulz)(void *, void *, zz_t) = a->parent->base->mulz;
      int_t size = a->parent->base->size;

      poly_fit(a, b->length);
      
      for (int_t i = 0; i < b->length; i++)
         mulz(a->coeffs + i*size, a->coeffs + i*size, c);

      a->length = b->length;
      poly_normalise(a);
   }
}

