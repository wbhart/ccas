#include "ccas.h"
#include "Poly.h"

void poly_set(Poly_t a, Poly_t b)
{
   if (a != b)
   {
      void (* set)(void *, void *) = a->parent->base->set;
      int_t size = a->parent->base->size;

      poly_fit(a, b->length);

      for (int_t i = 0; i < b->length; i++)
         set(a->coeffs + i*size, b->coeffs + i*size);

      a->length = b->length;
   }
}

