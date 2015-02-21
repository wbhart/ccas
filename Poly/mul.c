#include "ccas.h"
#include "Poly.h"

void poly_mul(Poly_t a, Poly_t b, Poly_t c)
{
   const int_t lenb = b->length, lenc = c->length;
   int_t lena = lenb + lenc - 1;
   int_t i, j;

   if (lenb == 0 || lenc == 0)
   {
      a->length = 0;

      return;
   }

   void (*init)(void *, parent_t) = a->parent->base->init;
   void (*clear)(void *) = a->parent->base->clear;
   void (* add)(void *, void *, void *) = a->parent->base->add;
   void (* mul)(void *, void *, void *) = a->parent->base->mul;
   int_t size = a->parent->base->size;
   TMP_INIT;
  
   poly_fit(a, lena);

   TMP_START;
   void * t = TMP_ALLOC(size);
   init(t, a->parent->base);

   for (i = 0; i < lenc; i++)
      mul(a->coeffs + i*size, b->coeffs, c->coeffs + i*size);

   for (i = 1; i < lenb; i++)
      mul(a->coeffs + (lenc + i - 1)*size, b->coeffs + i*size,
          c->coeffs + (lenc - 1)*size);

   for (i = 1; i < lenb; i++)
   {
      for (j = 0; j < lenc - 1; j++)
      {
         mul(t, b->coeffs + i*size, c->coeffs + j*size);
         add(a->coeffs + (i + j)*size, a->coeffs + (i + j)*size, t);
      }
   }

   a->length = lena;
   poly_normalise(a);

   clear(t);

   TMP_END;
}

