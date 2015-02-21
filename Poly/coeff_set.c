#include "ccas.h"
#include "Poly.h"

void poly_coeff_set(Poly_t pol, int_t n, void * coeff)
{
   void (* set)(void *, void *) = pol->parent->base->set;
   void (* zero)(void *) = pol->parent->base->zero;
   int (* iszero)(void *) = pol->parent->base->iszero;
   int_t size = pol->parent->base->size;

   if (n < pol->length || !iszero(coeff))
   {
      poly_fit(pol, n + 1);

      for (int_t i = pol->length; i < n; i++)
         zero(pol->coeffs + i*size);

      set(pol->coeffs + n*size, coeff);
   }

   if (n >= pol->length)
      pol->length = n + 1;   

   poly_normalise(pol);
}

