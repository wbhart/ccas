#include "ccas.h"
#include "nn.h"

void nn_div_pi1(nn_t q, uint_t cy, nn_t a, int_t m, nn_src_t d, int_t n, uint_t pi1)
{
   int_t qn = m - n + 1;
   TMP_INIT;
   
   TMP_START;
   if (qn + 1 >= n)
   {
      /* quotient not short, just do divrem and discard remainder */  
      nn_divrem_pi1(q, cy, a, m, d, n, pi1);
   } else 
   {
      /* quotient is short, try estimating and adjust */
      nn_t t1 = (nn_t) TMP_ALLOC((qn + 1)*sizeof(uint_t));
      
      /* develop one more quotient word than needed */
      nn_divrem_pi1(t1, cy, a + m - 2*qn - 1, 2*qn + 1, 
	                            d + n - qn - 1, qn + 1, pi1);
      nn_copyi(q, t1 + 1, qn);
      
      /* check extra quotient word */
      if (t1[0] == 0) 
      { 
	 /* quotient may be one too large */
         nn_t t2 = (nn_t) TMP_ALLOC((qn + n)*sizeof(uint_t));
      
	 /* multiply out */
         nn_mul(t2, d, n, q, qn);
         /* adjust if quotient too large */
	 if (nn_sub_m(a, a, t2, n))
            nn_sub_1(q, q, qn, 1);
      }
   }
   
   TMP_END;
}
