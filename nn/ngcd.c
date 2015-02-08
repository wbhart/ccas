#include "ccas.h"
#include "nn.h"

int_t nn_ngcd(nn_t a, int_t m, nn_t b, int_t n, nn_t * M, int_t * mn)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n > 0);
   CCAS_ASSERT(n > m/2 + 1);
   CCAS_ASSERT(!NN_OVERLAP(a, m, b, n));

   nn_t * M1 = nn_ngcd_mat_init(m/2);
   int_t qn, m1, mn1, s = m/2 + 1;
   int_t m0 = m, n0 = n;
   int sgn = 1;
   TMP_INIT;

   TMP_START;
   nn_t q = (nn_t) TMP_ALLOC(m*sizeof(uint_t));

   if (n > 3*m0/4 + 2)
   {
      int_t p1 = m0/2;

      m = nn_ngcd(a + p1, m - p1, b + p1, n - p1, M, mn);
      n = nn_normalise(b, m);

      m = nn_ngcd_mat_apply(a, m, b, n, p1, M, *mn);
      n = nn_normalise(b, m);
   } else
      nn_ngcd_mat_identity(M, mn);

   while (m > 3*m0/4 + 1 && nn_ngcd_sdiv_cmp(a, m, b, n, s) > 0)
   {
      nn_ngcd_sdiv(q, a, a, m, b, n, s);
      
      qn = nn_normalise(q, m - n + 1);
      
      nn_swap(a, b);
      m1 = n;
      n = nn_normalise(b, CCAS_MIN(m, n + 1));
      m = m1;
      sgn = -sgn;
      nn_ngcd_mat_update(M, mn, q, qn);
   }
   
   if (n > s + 2)
   {
      int_t p2 = 2*s - m + 1;

      m = nn_ngcd(a + p2, m - p2, b + p2, n - p2, M1, &mn1);
      n = nn_normalise(b, m);

      m = nn_ngcd_mat_apply(a, m, b, n, p2, M1, mn1);
      n = nn_normalise(b, m);

      nn_ngcd_mat_mul(M, mn, M1, mn1); 
   }

   while (nn_ngcd_sdiv_cmp(a, m, b, n, s) > 0)
   {
      nn_ngcd_sdiv(q, a, a, m, b, n, s);

      qn = nn_normalise(q, m - n + 1);

      nn_swap(a, b);
      m1 = n;
      n = nn_normalise(b, CCAS_MIN(m, n + 1));
      m = m1;
      sgn = -sgn;

      nn_ngcd_mat_update(M, mn, q, qn);
   }

   /* a and b have been swapped, so unswap */
   if (sgn < 0)
   {
      nn_t t = (nn_t) TMP_ALLOC(n*sizeof(uint_t));

      nn_copyi(t, b, n);
      nn_copyi(b, a, m);
      nn_copyi(b, t, n);
   }

   nn_ngcd_mat_clear(M1);

   TMP_END;

   return m;
}

