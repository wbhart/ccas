#include "ccas.h"
#include "nn.h"

void nn_getstr_tree_clear(nn_t * tree, int_t depth)
{
   for (int_t j = 0; j < depth; j++)
      free(tree[j]);
}
	
