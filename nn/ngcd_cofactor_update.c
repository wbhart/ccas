#include "ccas.h"
#include "nn.h"

int_t nn_ngcd_cofactor_update(nn_t s1, nn_t s2, int_t sn, nn_t * M, int_t mn)
{
   int_t m = CCAS_ABS(sn);
   int_t n = CCAS_ABS(mn);
   int_t n1, n2, n3;
   TMP_INIT;

   TMP_START;
   nn_t t1 = (nn_t) TMP_ALLOC((m + n)*sizeof(uint_t));
   nn_t t2 = (nn_t) TMP_ALLOC((m + n)*sizeof(uint_t));
   nn_t t3 = (nn_t) TMP_ALLOC((m + n)*sizeof(uint_t));
   nn_t t4 = (nn_t) TMP_ALLOC((m + n)*sizeof(uint_t));
   
   if (m >= n)
   {
      nn_mul(t1, s1, m, M[0], n);
      nn_mul(t2, s2, m, M[1], n);
      nn_mul(t3, s1, m, M[2], n);
      nn_mul(t4, s2, m, M[3], n);
   } else
   {
      nn_mul(t1, M[0], n, s1, m);
      nn_mul(t2, M[1], n, s2, m);
      nn_mul(t3, M[2], n, s1, m);
      nn_mul(t4, M[3], n, s2, m);
   }

   n3 = n2 = n1 = m + n;

   /* always update at least m words of s1 and s2 */
   while (n2 > m && t1[n2 - 1] == 0)
      n2--;

   while (n1 > n2 && t2[n1 - 1] == 0)
      n1--;

   if (nn_add_m(s1, t1, t2, n1))
      s1[n1++] = 1;
	  
   n2 = m + n;
   
   while (n3 > m && t3[n3 - 1] == 0)
      n3--;

   while (n2 > n3 && t4[n2 - 1] == 0)
      n2--;

   if (nn_add_m(s2, t3, t4, n2))
      s2[n2++] = 1;
   
   TMP_END;
   
   return (sn ^ mn) < 0 ? -CCAS_MAX(n1, n2) : CCAS_MAX(n1, n2);
}

