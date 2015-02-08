#include "ccas.h"
#include "nn.h"

nn_t * nn_ngcd_mat_init(int_t n)
{
   CCAS_ASSERT(n >= 0);

   nn_t M = malloc((4 + 4*n)*sizeof(uint_t));

   ((nn_t *) M)[0] = M + 4;
   ((nn_t *) M)[1] = M + 4 + n;
   ((nn_t *) M)[2] = M + 4 + 2*n;
   ((nn_t *) M)[3] = M + 4 + 3*n;

   nn_zero(((nn_t *) M)[0], n);
   nn_zero(((nn_t *) M)[1], n);
   nn_zero(((nn_t *) M)[2], n);
   nn_zero(((nn_t *) M)[3], n);

   return (nn_t *) M;
}
