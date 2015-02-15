#include "ccas.h"
#include "nn.h"

void nn_getstr_tree_init(nn_t * tree, int_t * tn, int_t depth)
{
   CCAS_ASSERT(depth >= 0);

   if (depth == 0)
      return;

   tree[0] = malloc(sizeof(uint_t));
   tree[0][0] = 10;
   tn[0] = 1;
  
   /* compute powers of 10 */
   for (int_t j = 1; j < depth; j++)
   {
      tree[j] = (nn_t) malloc(2*tn[j - 1]*sizeof(uint_t));
      nn_mul_m(tree[j], tree[j - 1], tree[j - 1], tn[j - 1]);
      tn[j] = nn_normalise(tree[j], 2*tn[j - 1]);
   }
}
	  
