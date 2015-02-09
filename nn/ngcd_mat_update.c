#include "ccas.h"
#include "nn.h"

void nn_ngcd_mat_update(nn_t * M, int_t * mn, nn_src_t q, int_t qn)
{
   CCAS_ASSERT(qn >= 0);
   
   int_t un = (*mn) >= 0 ? (*mn) : -(*mn);
   int_t rn = un + qn;
   int sgn = (*mn) >= 0 ? 1 : -1;
   TMP_INIT;

   TMP_START;

   nn_t t1 = (nn_t) TMP_ALLOC((rn + 1)*sizeof(uint_t));
   nn_t t2 = (nn_t) TMP_ALLOC((rn + 1)*sizeof(uint_t));

   if (un >= qn)
   {
      nn_mul(t1, M[2], un, q, qn);
      t1[rn] = nn_add(t1, t1, rn, M[0], un);

      nn_mul(t2, M[3], un, q, qn);
      t2[rn] = nn_add(t2, t2, rn, M[1], un);
   } else
   {
      nn_mul(t1, q, qn, M[2], un);
      t1[rn] = nn_add(t1, t1, rn, M[0], un);

      nn_mul(t2, q, qn, M[3], un);
      t2[rn] = nn_add(t2, t2, rn, M[1], un);
   }

   un = nn_normalise(t1, rn + 1);
   un = CCAS_MAX(un, nn_normalise(t2, rn + 1));

   nn_copyi(M[0], M[2], un);
   nn_copyi(M[1], M[3], un);
   nn_copyi(M[2], t1, un);
   nn_copyi(M[3], t2, un);

   TMP_END;

   (*mn) = sgn > 0 ? -un : un;
}

