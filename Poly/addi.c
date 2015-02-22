#include "ccas.h"
#include "Poly.h"

void poly_addi(Poly_t a, Poly_t b, int_t c)
{
   if (b->length == 0)
      poly_seti(a, c);
   else
   {
      void (* addi)(void *, void *, int_t) = a->parent->base->addi;

      poly_set(a, b);
      
      addi(a->coeffs, a->coeffs, c);

      poly_normalise(a);
   }
}

