#include "ccas.h"
#include "nn.h"

int_t nn_ngcd(nn_t a, int_t m, nn_t b, int_t n, nn_t * M, int_t * mn)
{
   CCAS_ASSERT(n > 0);
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n > m/2 + 1);
   CCAS_ASSERT(!NN_OVERLAP(a, m, b, n));

   TMP_INIT;

   TMP_START;
   nn_t q = (nn_t) TMP_ALLOC(m*sizeof(uint_t));
   int_t m1, qn;
   int sgn = 1;

   nn_t * M1;
   int_t mn1, s = m/2 + 1;
   int_t m0 = m, n0 = n;
      
   if (n > NGCD_THRESHOLD)
   {
      if (n > 3*m0/4 + 2 && nn_ngcd_sdiv_cmp(a, m, b, n, s))
      {
         int_t p1 = m0/2;

         m = p1 + nn_ngcd(a + p1, m - p1, b + p1, n - p1, M, mn);
         n = nn_normalise(b, m);

         m = nn_ngcd_mat_apply(a, m, b, n, p1, M, mn);
         n = nn_normalise(b, m); 
      }

      while (m > 3*m0/4 + 1 && nn_ngcd_sdiv_cmp(a, m, b, n, s))
      {
         nn_ngcd_sdiv(q, a, a, m, b, n, s);
      
         qn = nn_normalise(q, m - n + 1);
      
         nn_swap(a, b);
         m1 = n;
         /* sdiv remainder can be bigger than n words */
         n = nn_normalise(b, CCAS_MIN(m, n + 1));
         m = m1;
         sgn = -sgn;
         nn_ngcd_mat_update(M, mn, q, qn);
         
         /* special case, sdiv can leave m < n */
         if (m < n)
            ngcd_reorder(a, &m, b, &n, M, mn);
      }
   
      if (n > s + 2 && nn_ngcd_sdiv_cmp(a, m, b, n, s))
      {
         int_t p2 = 2*s - m + 1;
         M1 = nn_ngcd_mat_init((m - p2)/2);
         
         nn_ngcd_mat_identity(M1, &mn1);

         m = p2 + nn_ngcd(a + p2, m - p2, b + p2, n - p2, M1, &mn1);
         n = nn_normalise(b, m);

         m = nn_ngcd_mat_apply(a, m, b, n, p2, M1, &mn1);
         n = nn_normalise(b, m);

         nn_ngcd_mat_mul(M, mn, M1, mn1); 

         nn_ngcd_mat_clear(M1);
      }      
   }

   while (nn_ngcd_sdiv_cmp(a, m, b, n, s))
   {
      nn_ngcd_sdiv(q, a, a, m, b, n, s);
      qn = nn_normalise(q, m - n + 1);
      
      nn_swap(a, b);
      m1 = n;
      /* sdiv remainder can be bigger than n words */
      n = nn_normalise(b, CCAS_MIN(m, n + 1));
      m = m1;
      sgn = -sgn;
         
      nn_ngcd_mat_update(M, mn, q, qn);
      
      /* special case, sdiv can leave m < n */
      if (m < n)
         ngcd_reorder(a, &m, b, &n, M, mn);
   }

   /* a and b have been swapped, so unswap */
   if (sgn < 0)
   {
      nn_t t = (nn_t) TMP_ALLOC(n*sizeof(uint_t));

      nn_copyi(t, b, n);
      nn_copyi(b, a, m);
      nn_copyi(a, t, n);
      nn_zero(a + n, m - n);
   }
   
   TMP_END;

   return m;
}

