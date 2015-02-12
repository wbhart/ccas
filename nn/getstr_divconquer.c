#include "ccas.h"
#include "nn.h"

int_t nn_getstr_divconquer(char * str, int_t digits, nn_t a, 
                                       int_t m, nn_t * tree, int_t * tn)
{
   CCAS_ASSERT(m >= 0);
   CCAS_ASSERT(digits > 0);
   TMP_INIT;
   
   if (m == 0)
      return 0;
	  
   if (digits < GETSTR_DIVCONQUER_THRESHOLD)
      return nn_getstr_classical(str, digits, a, m);
   
   TMP_START;
   int_t depth = BITCOUNT(digits - 1);
   int_t d, qn = m - tn[depth - 1] + 1; 
   nn_t q = (nn_t) TMP_ALLOC(qn*sizeof(uint_t));
   nn_t r = (nn_t) TMP_ALLOC(tn[depth - 1]*sizeof(uint_t));
   int_t rn, half = (1UL << (depth - 1));
   
   /* divide number to write into quotient and remainder mod power of 10 */
   if (m >= tn[depth - 1])
      nn_divrem(q, r, a, m, tree[depth - 1], tn[depth - 1]);
   else
   {
      nn_copyi(r, a, m);
	  nn_zero(r + m, tn[depth - 1] - m);
	  nn_zero(q, qn);
   }
   
   qn = nn_normalise(q, qn);
   rn = nn_normalise(r, tn[depth - 1]);
   
   /* write low digits into string */
   d = nn_getstr_divconquer(str + digits - half, half, r, rn, tree, tn);
   
   /* write leading zeros */
   for (int_t j = 0; j < half - d; j++)
      str[digits - half + j] = 48;
   
   /* write high digits into string */
   d = nn_getstr_divconquer(str, digits - half, q, qn, tree, tn);
   
   return half + d;
}

