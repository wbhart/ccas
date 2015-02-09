#include "ccas.h"
#include "nn.h"

int_t nn_gcd_ngcd(nn_t g, nn_src_t a, int_t m, nn_src_t b, int_t n)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n > 0);

   TMP_INIT;

   TMP_START;
   nn_t q = (nn_t) TMP_ALLOC(m*sizeof(uint_t));
   nn_t r1 = (nn_t) TMP_ALLOC(m*sizeof(uint_t));
   nn_t r2 = (nn_t) TMP_ALLOC(n*sizeof(uint_t));

   nn_copyi(r1, a, m);
   nn_copyi(r2, b, n);
   nn_zero(r2 + n, m - n);
 
   /* a = bq + r; a = b; b = r; */
   if (m > n)
   {
      nn_divrem(q, r1, r1, m, r2, n);
      m = n;
      n = nn_normalise(r1, n);
      nn_swap(r1, r2);
   }

   nn_t * M = nn_ngcd_mat_init(m/2);
   
   while (n > NGCD_THRESHOLD)
   {
      nn_zero(M[0], m/2);
      nn_zero(M[1], m/2);
      nn_zero(M[2], m/2);
      nn_zero(M[3], m/2);

      int_t mn, p1 = m/2;
      
      nn_ngcd_mat_identity(M, &mn);

      m = p1 + nn_ngcd(r1 + p1, m - p1, r2 + p1, n - p1, M, &mn);
      n = nn_normalise(r2, m);

      /* make sure something happened */
      if (!nn_ngcd_mat_is_identity(M, mn))
      {
         m = nn_ngcd_mat_apply(r1, m, r2, n, p1, M, mn);
         n = nn_normalise(r2, m);
      } else
      {
         nn_divrem(q, r1, r1, m, r2, n);
         nn_swap(r1, r2);
         int_t m = n;
         n = nn_normalise(r2, n);
      }
   }
 
   nn_ngcd_mat_clear(M);

   /* a = bq + r; a = b; b = r; */
   while (n != 0)
   {
      nn_divrem(q, r1, r1, m, r2, n);
      m = n;
      n = nn_normalise(r1, n);
      nn_swap(r1, r2);
   }
   
   /* write out GCD */
   nn_copyi(g, r1, m);
   
   TMP_END;

   return m;
}

