#include <string.h>
#include <math.h>
#include "ccas.h"
#include "zz.h"

int_t zz_setstr(zz_ptr a, const char * str)
{
   int negative = str[0] == '-';

   if (negative)
      str++;

   int_t digits = strspn(str, "0123456789");

   /* 0.103810253 is log(10/log(2^32) */
   int_t words = (int_t) ceil(digits*0.103810253*(INT_BITS/32));

   zz_fit(a, words);

   digits = nn_setstr(a->n, &(a->size), str);

   if (negative)
   {
      zz_neg(a, a);
      digits++;
   } 

   return digits;
}

