#include <string.h>
#include "ccas.h"
#include "nn.h"

int_t nn_setstr(nn_t a, int_t * len, const char * str)
{
   int_t i, m = 1, digits = strspn(str, "0123456789");
   uint_t cy;

   if (digits == 1 && str[0] == '0')
   {
      (*len) = 0;

      return 1;
   }

   a[0] = (uint_t) str[0] - 48;
   
   for (i = 1; i < digits; i++) {
      cy = nn_mul_1(a, a, m, 10);
      cy += nn_add_1(a, a, m, (uint_t) str[i] - 48);
      
      if (cy)
         a[m++] = cy;
   }

   (*len) = m;

   return digits;
}

