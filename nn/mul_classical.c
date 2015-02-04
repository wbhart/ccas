#include "ccas.h"
#include "nn.h"

void nn_mul_classical(nn_t r, nn_src_t a, int_t m, nn_src_t b, int_t n)
{
   CCAS_ASSERT(n >= 0);
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(!NN_OVERLAP(r, m + n, a, m));
   CCAS_ASSERT(!NN_OVERLAP(r, m + n, b, n));

   if (n > 0)
      r[m] = nn_mul_1(r, a, m, b[0]); 
   else
      nn_zero(r, m);

   for (int_t i = 1; i < n; i++)
      r[m + i] = nn_addmul_1(r + i, a, m, b[i]);
}

