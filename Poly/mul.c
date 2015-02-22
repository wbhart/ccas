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

   void (* init)(void *, parent_t) = a->parent->base->init;
   void (* clear)(void *) = a->parent->base->clear;
   void (* add)(void *, void *, void *) = a->parent->base->add;
   void (* mul)(void *, void *, void *) = a->parent->base->mul;
   int (* iszero)(void *) = a->parent->base->iszero;
   int_t size = a->parent->base->size;
   TMP_INIT;
  
   TMP_START;
   void * t = TMP_ALLOC(size);
   init(t, a->parent->base);
   
   Poly_t temp;
   Poly_struct * r = a;

   if (a == b || a == c)
   {
      poly_init(temp, a->parent);
      r = temp;
   }

   poly_fit(r, lena);

   for (i = 0; i < lenc; i++)
      mul(r->coeffs + i*size, b->coeffs, c->coeffs + i*size);

   for (i = 1; i < lenb; i++)
      mul(r->coeffs + (lenc + i - 1)*size, b->coeffs + i*size,
          c->coeffs + (lenc - 1)*size);

   for (i = 1; i < lenb; i++)
   {
      for (j = 0; j < lenc - 1; j++)
      {
         mul(t, b->coeffs + i*size, c->coeffs + j*size);
         add(r->coeffs + (i + j)*size, r->coeffs + (i + j)*size, t);
      }
   }

   r->length = lena;
   poly_normalise(r);

   if (a == b || a == c)
   {
      poly_swap(a, r);
      poly_clear(temp);
   }

   clear(t);

   TMP_END;
}

