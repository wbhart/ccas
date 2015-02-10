#include "ccas.h"
#include "nn.h"

int_t nn_xgcd_ngcd(nn_t g, nn_t s, int_t * sn, nn_src_t a, int_t m,
                                                 nn_src_t b, int_t n)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n > 0);
   CCAS_ASSERT(!NN_OVERLAP(g, n, s, n));

   TMP_INIT;

   TMP_START;
   nn_t q = (nn_t) TMP_ALLOC(m*sizeof(uint_t));
   nn_t r1 = (nn_t) TMP_ALLOC(m*sizeof(uint_t));
   nn_t r2 = (nn_t) TMP_ALLOC(n*sizeof(uint_t));
   nn_t s2 = (nn_t) TMP_ALLOC(n*sizeof(uint_t));
   int_t mn, qn;
   
   nn_copyi(r1, a, m);
   nn_copyi(r2, b, n);
   nn_zero(r2 + n, m - n);
   nn_zero(s, n);
   nn_zero(s2, n);
   
   /* a = bq + r; a = b; b = r; */
   if (m > n)
   {
      nn_divrem(q, r1, r1, m, r2, n);
      m = n;
      n = nn_normalise(r1, n);
      nn_swap(r1, r2);
	  
	  /* initialise cofactor vector (0, 1)*/
	  s2[0] = 1;
	  (*sn) = -1;
   } else
   {
      /* initialise cofactor vector (1, 0) */
	  s[0] = 1;
	  (*sn) = 1;
   }

   nn_t * M = nn_ngcd_mat_init(m);
   
   while (n > NGCD_THRESHOLD)
   {
      int_t p1 = m/2;
      
      nn_zero(M[0], (m + 1)/4);
      nn_zero(M[1], (m + 1)/4);
      nn_zero(M[2], (m + 1)/4);
      nn_zero(M[3], (m + 1)/4);
  
      nn_ngcd_mat_identity(M, &mn);

      /* this ensures n > s in ngcd */
      if (n - p1 > (m - p1)/2 + 1)
      { 
         m = p1 + nn_ngcd(r1 + p1, m - p1, r2 + p1, n - p1, M, &mn);
         n = nn_normalise(r2, m);
      }
      
      /* make sure something happened */
      if (!nn_ngcd_mat_is_identity(M, mn))
      {
         m = nn_ngcd_mat_apply(r1, m, r2, n, p1, M, &mn);
         n = nn_normalise(r2, m);
      } else
      {
         nn_divrem(q, r1, r1, m, r2, n);
         qn = nn_normalise(q, m - n + 1);
		 nn_swap(r1, r2);
         m = n;
         n = nn_normalise(r2, n);
		 
		 nn_ngcd_mat_update(M, &mn, q, qn);
      }
	  
	  (*sn) = nn_ngcd_cofactor_update(s, s2, *sn, M, mn);
   }
 
   /* a = bq + r; a = b; b = r; */
   while (n != 0)
   {
      nn_zero(M[0], n);
      nn_zero(M[1], n);
      nn_zero(M[2], n);
      nn_zero(M[3], n);
  
      nn_ngcd_mat_identity(M, &mn);

	  nn_divrem(q, r1, r1, m, r2, n);
      qn = nn_normalise(q, m - n + 1);
	  m = n;
      n = nn_normalise(r1, n);
      nn_swap(r1, r2);
	  
	  nn_ngcd_mat_update(M, &mn, q, qn);
	  (*sn) = nn_ngcd_cofactor_update(s, s2, *sn, M, mn);
   }
   
   nn_ngcd_mat_clear(M);

   /* write out GCD */
   nn_copyi(g, r1, m);
   
   TMP_END;

   return m;
}

