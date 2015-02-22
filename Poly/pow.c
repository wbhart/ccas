#include "ccas.h"
#include "Poly.h"

void poly_pow(Poly_t a, Poly_t b, int_t n)
{
   Poly_t temp;
   Poly_struct * r = a;

   if (n == 0)
      poly_seti(a, 1);
   else if (b->length == 0)
      a->length = 0;
   else if (n == 1)
      poly_set(a, b);
   else
   {
      if (a == b)
      {
         poly_init(temp, a->parent);
         r = temp;
      }

      uint_t bit = ~((~(uint_t) 0) >> 1);
   
      while ((bit & n) == 0)
         bit >>= 1;

      poly_set(r, b);

      bit >>= 1;

      while (bit != 0)
      {
         poly_mul(r, r, r);

         if ((bit & n) != 0)
            poly_mul(r, r, b);
       
         bit >>= 1;
      }

      if (a == b)
      {
         poly_swap(a, r);
         poly_clear(temp);
      }
   }
}

