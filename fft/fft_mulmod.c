#include "ccas.h"
#include "fft.h"

void fft_mulmod(nn_t rr, nn_src_t ii, nn_src_t jj, int_t limbs)
{
   CCAS_ASSERT(limbs >= 0);

   TMP_INIT;

   TMP_START;

   nn_t tt = (nn_t) TMP_ALLOC((2*limbs)*sizeof(uint_t));

   uint_t hi1 = ii[limbs];
   uint_t hi2 = jj[limbs];

   nn_mul_classical(tt, ii, limbs, jj, limbs);
   
   if (hi1 == 1)
      nn_add(tt + limbs, tt + limbs, limbs, jj, limbs);

   if (hi2 == 1)
      nn_add(tt + limbs, tt + limbs, limbs, ii, limbs);

   rr[limbs] = -nn_sub_m(rr, tt, tt + limbs, limbs);

   /* -1 * -1 = 1 mod p */
   if (hi1 == 1 && hi2 == 1)
      rr[0] = 1;

   TMP_END;
}
