#include "ccas.h"
#include "nn.h"

int_t nn_setstr_classical(nn_t a, const char * str, int_t digits)
{
   CCAS_ASSERT(digits >= 0);

   int_t m = 1;
   uint_t cy;

   if (digits == 0)
      return 0;

   a[0] = (uint_t) str[0] - 48;
   
   for (int_t i = 1; i < digits; i++)
   {
      cy = nn_mul_1(a, a, m, 10);
      cy += nn_add_1(a, a, m, (uint_t) str[i] - 48);
      
      if (cy)
         a[m++] = cy;
   }

   return m;
}

