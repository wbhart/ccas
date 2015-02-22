#include "ccas.h"
#include "Poly.h"

void poly_pow(Poly_t a, Poly_t b, int_t n)
{
   Poly_t temp;
   Poly_struct * r = a;

   if (a == b)
   {
      poly_init(temp, a->parent);
      r = temp;
   }

   uint_t bit = ~((~(uint_t) 0) >> 1);
   
   while (((uint_t) bit & n) == 0)
      bit >>= 1;

   poly_set(r, b);

   bit >>= 1;

   while (bit != 0)
   {
      poly_mul(r, r, r);

      if (((uint_t) bit & n) != 0)
         poly_mul(r, r, b);
       
      bit >>= 1;
   }

   if (a == b)
   {
      poly_swap(a, r);
      poly_clear(temp);
   }
}

