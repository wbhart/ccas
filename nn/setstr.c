#include <string.h>
#include "ccas.h"
#include "nn.h"

int_t nn_setstr(nn_t a, int_t * len, const char * str)
{
   int_t i, m = 1, digits = strspn(str, "0123456789");
   int_t depth = BITCOUNT(digits - 1);
   TMP_INIT;
   
   if (digits == 1 && str[0] == '0')
   {
      (*len) = 0;

      return 1;
   }
   
   TMP_START;
   nn_t * tree = (nn_t *) TMP_ALLOC(depth*sizeof(nn_t));
   int_t * tn = (int_t *) TMP_ALLOC(depth*sizeof(int_t));
      
   /* initialise powers of 10 up to 10^depth */
   nn_decimal_tree_init(tree, tn, depth);

   (*len) = nn_setstr_divconquer(a, str, digits, tree, tn);
   
   /* clean up tree structure */
   nn_decimal_tree_clear(tree, depth);
	  
   TMP_END;

   return digits;
}

