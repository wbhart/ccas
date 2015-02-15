#include <math.h>
#include "ccas.h"
#include "nn.h"

char * nn_getstr(nn_t a, int_t m)
{
   CCAS_ASSERT(m >= 0);

   /* 9.63... is log_10(2^32) */
   int_t i = 0, j;
   int_t digits = (int_t) ceil(m*9.632959861247398*(INT_BITS/32)) + (m == 0);
   int_t depth = BITCOUNT(digits - 1);
   char * str = (char *) malloc(digits + 1);
   TMP_INIT;
   
   if (m == 0)
      str[0] = '0';
   else 
   {
      TMP_START;
	  nn_t * tree = (nn_t *) TMP_ALLOC(depth*sizeof(nn_t));
      int_t * tn = (int_t *) TMP_ALLOC(depth*sizeof(int_t));
      
	  /* initialise powers of 10 up to 10^depth */
	  nn_getstr_tree_init(tree, tn, depth);
	  
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
	  nn_getstr_tree_clear(tree, depth);
	  
	  TMP_END;
   }

   str[digits - i] = '\0';

   return str;
}

