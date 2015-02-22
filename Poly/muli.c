#include "ccas.h"
#include "Poly.h"

void poly_muli(Poly_t a, Poly_t b, int_t c)
{
   if (b->length == 0 || c == 0)
      a->length = 0;
   else if (c == 1)
      poly_set(a, b);
   else if (c == -(int_t) 1)
      poly_neg(a, b);
   else
   {
      void (* muli)(void *, void *, int_t) = a->parent->base->muli;
      int_t size = a->parent->base->size;

      poly_fit(a, b->length);
      
      for (int_t i = 0; i < b->length; i++)
         muli(a->coeffs + i*size, a->coeffs + i*size, c);

      a->length = b->length;
      poly_normalise(a);
   }
}

