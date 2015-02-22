#include "ccas.h"
#include "Poly.h"

void poly_seti(Poly_t a, int_t n)
{
   if (n == 0)
      a->length = 0;
   else
   {
      void (* seti)(void *, int_t) = a->parent->base->seti;

      poly_fit(a, 1);

      seti(a->coeffs, n);

      a->length = 1;
   }
}

