#include "ccas.h"
#include "zz.h"

void zz_subi(zz_ptr r, zz_srcptr a, int_t c)
{
   int_t usize = CCAS_ABS(a->size);

   zz_fit(r, usize + 1);
   
   if (a->size == 0)
   {
      r->size = -1;
      r->n[0] = c;
   } else
   {
      if (a->size < 0)
      {
         r->n[usize] = nn_add_1(r->n, a->n, usize, c);
         r->size = -nn_normalise(r->n, usize + 1);
      } else if (usize == 1)
      {
         uint_t d = a->n[0];

         if (d == c)
            r->size = 0;
         else
         {
            r->n[0] = d > c ? d - c : c - d;
            r->size = d > c ? 1 : -1;
         }
      } else
      {
         nn_sub_1(r->n, a->n, usize, c);
         r->size = nn_normalise(r->n, usize);
      }
   }
}

