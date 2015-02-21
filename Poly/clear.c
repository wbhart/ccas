#include "ccas.h"
#include "Poly.h"

void poly_clear(Poly_t pol)
{
   void (* clear)(void *) = pol->parent->base->clear;
   int_t size = pol->parent->base->size;

   for (int_t i = 0; i < pol->alloc; i++)
      clear(pol->coeffs + i*size);

   free(pol->coeffs);
}

