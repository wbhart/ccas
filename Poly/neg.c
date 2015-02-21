#include "ccas.h"
#include "Poly.h"

void poly_neg(Poly_t a, Poly_t b)
{
   void (* neg)(void *, void *) = a->parent->base->neg;
   int_t size = a->parent->base->size;

   poly_fit(a, b->length);

   for (int_t i = 0; i < b->length; i++)
      neg(a->coeffs + i*size, b->coeffs + i*size);

   a->length = b->length;
}

