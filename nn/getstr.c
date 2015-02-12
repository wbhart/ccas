#include <math.h>
#include "ccas.h"
#include "nn.h"

char * nn_getstr(nn_t a, int_t m)
{
   CCAS_ASSERT(m >= 0);

   /* 9.63... is log_10(2^32) */
   int_t i = 0, j;
   int_t digits = (int_t) ceil(m*9.632959861247398*(INT_BITS/32)) + (m == 0);
   int_t n, depth = BITCOUNT(digits - 1);
   char * str = (char *) malloc(digits + 1);
   TMP_INIT;
   
   if (m == 0)
      str[0] = '0';
   else 
   {
      TMP_START;
	  nn_t * tree = (nn_t *) TMP_ALLOC(depth*sizeof(nn_t));
      int_t * tn = (int_t *) TMP_ALLOC(depth*sizeof(int_t));
      
	  n = 1;
	  tree[0] = malloc(sizeof(uint_t));
      tree[0][0] = 10;
	  tn[0] = 1;
	  
	  /* compute powers of 10 */
	  for (j = 1; j < depth; j++)
	  {
		 tree[j] = (nn_t) malloc(2*tn[j - 1]*sizeof(uint_t));
         nn_mul_m(tree[j], tree[j - 1], tree[j - 1], tn[j - 1]);
		 tn[j] = nn_normalise(tree[j], 2*tn[j - 1]);
	  }
	  
	  /* write digits of {a, m} into string */
	  i = digits - nn_getstr_divconquer(str, digits, a, m, tree, tn);

      /* skip any leading zeros printed */
	  while (str[i] == 48)
	     i++;
		 
      /* didn't get the right number of digits, shift */
      if (i != 0)
         for (j = i; j < digits; j++)
            str[j - i] = str[j];
			
	  /* clean up tree structure */
	  for (j = 0; j < depth; j++)
	     free(tree[j]);
		 
	  TMP_END;
   }

   str[digits - i] = '\0';

   return str;
}

