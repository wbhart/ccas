#include <math.h>
#include "ccas.h"
#include "nn.h"

char * nn_getstr(nn_t a, int_t m)
{
   CCAS_ASSERT(m >= 0);

   /* 9.63... is log_10(2^32) */
   int_t i = 0, j;
   int_t digits = (int_t) ceil(m*9.632959861247398*(INT_BITS/32)) + (m == 0);
   int_t norm = INT_BITS - 4; /* number of leading zero bits in 10UL */
   char * str = (char *) malloc(digits + 1);
   nn_t q1, q2;
   TMP_INIT;

   if (m == 0)
      str[0] = '0';
   else 
   {
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

      /* didn't get the right number of digits, shift */
      if (i != 0)
         for (j = i; j < digits; j++)
            str[j - i] = str[j];
   }

   str[digits - i] = '\0';

   return str;
}

