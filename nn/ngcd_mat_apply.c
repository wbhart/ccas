#include "ccas.h"
#include "nn.h"

int_t nn_ngcd_mat_apply(nn_t a, int_t m, nn_t b, int_t n, 
                                int_t p1, nn_t * M, int_t mn)
{
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n >= 0);
   CCAS_ASSERT(p1 < n);
   CCAS_ASSERT(!NN_OVERLAP(a, m, b, n));

   int_t pn1, pn2, un = mn >= 0 ? mn : -mn;
   int sgn = mn >= 0 ? 1 : -1;
   uint_t bw1, bw2, cy;
   TMP_INIT;

   TMP_START;

   nn_t t = (nn_t) TMP_ALLOC((p1 + un)*sizeof(uint_t));
   nn_t u = (nn_t) TMP_ALLOC((p1 + un)*sizeof(uint_t));
   nn_t v = (nn_t) TMP_ALLOC((p1 + un)*sizeof(uint_t));

   nn_mul(t, a, p1, M[3], un);
   nn_mul(u, b, p1, M[1], un);
   if ((bw1 = -nn_sub_m(t, t, u, p1 + un)))
      nn_neg(t, t, p1 + un);
   pn1 = nn_normalise(t, p1 + un);

   nn_mul(u, a, p1, M[2], un);
   nn_mul(v, b, p1, M[0], un);
   if ((bw2 = -nn_sub_m(u, v, u, p1 + un)))
      nn_neg(u, u, p1 + un);
   pn2 = nn_normalise(u, p1 + un);

   if ((sgn ^ bw1) < 0)
   {
      nn_sub(a, a, m, t, pn1);
      m = nn_normalise(a, m);
   } else
   {
      if (m >= pn1)
         cy = nn_add(a, a, m, t, pn1);
      else
      {
         cy = nn_add(a, t, pn1, a, m);
         m = pn1;
      }
      if (cy != 0)
        a[m++] = cy;
   }

   if ((sgn ^ bw2) > 0)
   {
      nn_sub(b, b, n, u, pn2);
      n = nn_normalise(b, n);
   } else
   {
      if (n >= pn2)
         cy = nn_add(b, b, n, u, pn2);
      else
      {
         cy = nn_add(b, u, pn2, b, n);
         n = pn2;
      }
      if (cy != 0)
        b[n++] = cy;
   }

   TMP_END; 

   return m;  
}

