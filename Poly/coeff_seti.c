#include "ccas.h"
#include "Poly.h"

void poly_coeff_seti(Poly_t pol, int_t n, int_t c)
{
   void (* seti)(void *, int_t) = pol->parent->base->seti;
   void (* zero)(void *) = pol->parent->base->zero;
   int_t size = pol->parent->base->size;

   if (n < pol->length || c != 0)
   {
      poly_fit(pol, n + 1);

      for (int_t i = pol->length; i < n; i++)
         zero(pol->coeffs + i*size);

      seti(pol->coeffs + n*size, c);
   }

   if (n >= pol->length)
      pol->length = n + 1;   

   poly_normalise(pol);
}

