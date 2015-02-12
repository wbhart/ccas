#include <math.h>
#include "ccas.h"
#include "nn.h"

int_t nn_getstr_classical(char * str, int_t digits, nn_t a, int_t m)
{
   CCAS_ASSERT(m >= 0);
   CCAS_ASSERT(digits > 0);

   nn_t q1, q2;
   int_t i;
   TMP_INIT;

   TMP_START;

   q1 = TMP_ALLOC(m*sizeof(uint_t));
   q2 = TMP_ALLOC(m*sizeof(uint_t));

   nn_copyi(q1, a, m);

   /* compute digits in reverse order */
   for (i = digits; m > 0; i--)
   {
      /* next digit is remainder upon division by 10 */
      str[i - 1] = 48 + (char) nn_divrem_1(q2, 0, q1, m, 10);
         
      nn_swap(q1, q2);
      m = nn_normalise(q1, m);
   }

   TMP_END;

   return digits - i;
}

