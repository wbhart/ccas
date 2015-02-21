#include "ccas.h"
#include "Poly.h"

void poly_add(Poly_t a, Poly_t b, Poly_t c)
{
   const int_t lenb = b->length, lenc = c->length;
   int_t lena = CCAS_MAX(lenb, lenc);
   int_t i;

   void (* add)(void *, void *, void *) = a->parent->base->add;
   void (* set)(void *, void *) = a->parent->base->set;
   int_t size = a->parent->base->size;

   poly_fit(a, lena);

   for (i = 0; i < CCAS_MIN(lenb, lenc); i++)
      add(a->coeffs + i*size, b->coeffs + i*size, c->coeffs + i*size);

   for ( ; i < lenb; i++)
      set(a->coeffs + i*size, b->coeffs + i*size);

   for ( ; i < lenc; i++)
      set(a->coeffs + i*size, c->coeffs + i*size);

   a->length = lena;
   poly_normalise(a);
}

