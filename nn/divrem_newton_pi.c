#include "ccas.h"
#include "nn.h"

void nn_divrem_newton_pi(nn_t q, uint_t cy, nn_t a, int_t m, 
                                         nn_src_t d, int_t n, nn_t dinv)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(m <= 2*n - 1);
   CCAS_ASSERT(n >= 2);
   CCAS_ASSERT(NORMALISED(d[n - 1]));
   CCAS_ASSERT(!NN_OVERLAP(q, m - n + 1, a, m));
   CCAS_ASSERT(!NN_OVERLAP(q, m - n + 1, d, n));
   CCAS_ASSERT(!NN_OVERLAP(a, m, d, n));
   
   int_t qn;
   TMP_INIT;
   
   /* 
      we use that d*dinv < B^2n < d(dinv + 4)
	  this translates to Q' < a/d < Q' + 6 where
	  Q' = floor((cy, {a + n, m - n})*dinv/B^n) so long
	  as m < 2n.
   */
   TMP_START;
   nn_t t = (nn_t) TMP_ALLOC(2*n*sizeof(uint_t));
   nn_t u = (nn_t) TMP_ALLOC(2*n*sizeof(uint_t));
   
   for ( ; m >= n; m -= n)
   {
      int_t qn = m - n + 1;
	  int_t q1 = CCAS_MIN(qn, n); /* quotient limbs computed this iteration */
	  
	  /* compute approx. quotient a_hi*dinv/B^n */
	  nn_mul(t, dinv, n, a + m - n + 1, q1 - 1);
	  t[n + q1 - 1] = nn_addmul_1(t + q1 - 1, d, n, cy);
	  nn_copyi(q + qn - q1, t + n, q1);
	  
	  /* compute remainder */
	  nn_mul(u, d, n, t + n, q1);
	  nn_sub_m(a +  m - n - q1 + 1, a + m - n - q1 + 1, u, n + 1);
	  
	  /* while remainder too large, increase quotient */
	  while (a[m - q1 + 1] != 0 || nn_cmp(a + m - n - q1 + 1, d, n) >= 0)
	  {
	     nn_add_1(q + qn - q1, q + qn - q1, q1, 1);
		 nn_sub_m(a + m - n - q1 + 1, a + m - n - q1 + 1, d, n);
	  }
	  
	  cy = a[m - q1];
   }
   	  
   TMP_END;
}

