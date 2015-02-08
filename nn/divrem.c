#include "ccas.h"
#include "nn.h"
#include "wide.h"

void nn_divrem(nn_t q, nn_t r, nn_src_t a, int_t m, nn_src_t d, int_t n)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n > 0);
   
   uint_t cy, pi1;
   int_t norm = INT_BITS - BITCOUNT(d[n - 1]);
   TMP_INIT;
   
   TMP_START;
   nn_t a2 = (nn_t) TMP_ALLOC(m*sizeof(uint_t));
   nn_t d2 = (nn_t) TMP_ALLOC(n*sizeof(uint_t));
   nn_t dinv;
   
   /* shift dividend and divisor left to normalise divisor */
   cy = nn_shl(a2, a, m, norm);
   nn_shl(d2, d, n, norm);
   
   /* precompute inverses */
   pi1 = preinvert1(d[n - 1]);
   
   int_t qn = m - n + 1;
   
   if (qn >= DIVREM_NEWTON_THRESHOLD && qn >= DIVREM_NEWTON_THRESHOLD)
   {
      dinv = (nn_t) TMP_ALLOC(n*sizeof(uint_t));
      nn_invert_pi1(dinv, d2, n, pi1);
   }

   /* ensure m < 2*n */
   for ( ; m >= 2*n; m -= n, qn -= n)
   {
      if (qn < DIVREM_DIVCONQUER_THRESHOLD)
	     nn_divrem_classical_pi1(q + qn - n, cy, a2 + m - 2*n + 1, 2*n - 1, d2, n, pi1);
	  else if (qn < DIVREM_NEWTON_THRESHOLD || n < DIVREM_NEWTON_THRESHOLD)
	     nn_divrem_divconquer_pi1(q + qn - n, cy, a2 + m - 2*n + 1, 2*n - 1, d2, n, pi1);
	  else
	     nn_divrem_newton_pi(q + qn - n, cy, a2 + m - 2*n + 1, 2*n - 1, d2, n, dinv);
		 
	  cy = a2[m - n];
   }   
   
   /* do division with m < 2*n */
   
   if (qn < DIVREM_DIVCONQUER_THRESHOLD)
	  nn_divrem_classical_pi1(q, cy, a2, m, d2, n, pi1);
   else if (qn < DIVREM_NEWTON_THRESHOLD || n < DIVREM_NEWTON_THRESHOLD)
	  nn_divrem_divconquer_pi1(q, cy, a2, m, d2, n, pi1);
   else
	  nn_divrem_newton_pi(q, cy, a2, m, d2, n, dinv);   
   
   /* shift remainder right */
   nn_shr(r, a2, n, norm);
   
   TMP_END;
}

