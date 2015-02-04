#include "ccas.h"
#include "nn.h"

void nn_invert_pi1(nn_t x, nn_src_t d, int_t n, uint_t pi1)
{
   CCAS_ASSERT(n > 0);
   CCAS_ASSERT(NORMALISED(d[n - 1]));
   CCAS_ASSERT(!NN_OVERLAP(x, n, d, n));
   TMP_INIT;
   
   if (n == 1)
      x[0] = pi1;
   else
   {
      int_t hi = (n + 1)/2;
	  int_t lo = n - hi;
      uint_t cy;
	  
	  /* D_hi*X_hi < B^{2*hi} < D_hi(X_hi + 4) */
      nn_invert_pi1(x + lo, d + lo, hi, pi1);
	  
	  TMP_START;
	  nn_t t = (nn_t) TMP_ALLOC((n + hi)*sizeof(uint_t));
	  nn_t u = (nn_t) TMP_ALLOC(2*hi*sizeof(uint_t));
	  
	  /* compute D*X_hi */
	  nn_mul(t, d, n, x + lo, hi);
      cy = nn_add_m(t + hi, t + hi, d, n);

	  /* ensure D*X_hi >= B^{2n} */
	  while (cy == 0)
	  {
	     nn_add_1(x + lo, x + lo, hi, 1);
	     cy += nn_add(t, t, n + hi, d, n);
	  }
	  
	  /* ensure D*X_hi < B^{2n} <= D(X_hi + 1) */
	  while (cy)
	  {
	     nn_sub_1(x + lo, x + lo, hi, 1);
	     cy -= nn_sub(t, t, n + hi, d, n);
	  }
	  
	  /* newton iteration x' = x + x(1 - dx) */
	  nn_neg(t, t, n);
      nn_mul(u, t + lo, hi, x + lo, hi);
      nn_add_m(u + hi, u + hi, t + lo, hi);
      nn_copyi(x, u + 2*hi - lo, lo);

      TMP_END;	  
   }
}
