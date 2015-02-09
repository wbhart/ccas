#include "ccas.h"
#include "nn.h"

/*
   We split {a, m} into a1, a2 and {b, n} into b1, b2, with a2 and b2
   both of length m2 = (m + 1)/2 (so that a1 and b1 don't exceed this
   length. We then compute s1 = a1 + a2, s2 = b1 + b2. We then compute
   t = s1*s1, u = a1*b1 and v = a2*b2. Finally we compute
   p = v + (t - u - v)*2^(INT_BITS*m2) + u*2^(2*INT_BITS*m2).
*/
void nn_mul_karatsuba(nn_t p, nn_src_t a, int_t m, nn_src_t b, int_t n)
{
   CCAS_ASSERT(!NN_OVERLAP(p, m + n, a, m));
   CCAS_ASSERT(!NN_OVERLAP(p, m + n, b, n));
   CCAS_ASSERT(n >= 0);
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n >= (m + 1)/2);

   int_t m2 = (m + 1)/2;
   int_t h1 = m - m2;
   int_t h2 = n - m2;
   uint_t cy;

   nn_t t;
   TMP_INIT;

   p[m2]       = nn_add(p, a, m2, a + m2, h1);
   p[2*m2 + 1] = nn_add(p + m2 + 1, b, m2, b + m2, h2);
   
   TMP_START;
   t = (nn_t) TMP_ALLOC((2*m2 + 2)*sizeof(uint_t));
   
   nn_mul_m(t, p + m2 + 1, p, m2 + 1); 
   nn_mul_m(p, a, b, m2);
   nn_mul(p + 2*m2, a + m2, h1, b + m2, h2);

   nn_sub(t, t, 2*m2 + 2, p, 2*m2);
   nn_sub(t, t, 2*m2 + 2, p + 2*m2, h1 + h2);

   nn_add(p + m2, p + m2, m + h2, t, CCAS_MIN(2*m2 + 2, m + h2));

   TMP_END;
}

