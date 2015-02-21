#include "ccas.h"
#include "Poly.h"

void poly_fit(Poly_t pol, int_t len)
{
   if (len > pol->alloc)
   {
      void (* init)(void *, parent_t) = pol->parent->base->init;
      int_t size = pol->parent->base->size;

      len = CCAS_MAX(len, 2*pol->alloc);

      pol->coeffs = realloc(pol->coeffs, size*len);
      
      for (int_t i = pol->alloc; i < len; i++)
         init(pol->coeffs + i*size, pol->parent->base);

      pol->alloc = len;
   }
}

