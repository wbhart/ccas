#include "ccas.h"
#include "Poly.h"

void poly_coeff_setz(Poly_t pol, int_t n, zz_t coeff)
{
   void (* setz)(void *, zz_t) = pol->parent->base->setz;
   void (* zero)(void *) = pol->parent->base->zero;
   int_t size = pol->parent->base->size;

   if (n < pol->length || !zz_iszero(coeff))
   {
      poly_fit(pol, n + 1);

      for (int_t i = pol->length; i < n; i++)
         zero(pol->coeffs + i*size);

      setz(pol->coeffs + n*size, coeff);
   }

   if (n >= pol->length)
      pol->length = n + 1;   

   poly_normalise(pol);
}

