#include "ccas.h"
#include "nn.h"

void nn_ngcd_mat_mul(nn_t * M1, int_t * m1, nn_t * M2, int_t m2)
{
   int_t un1 = (*m1) > 0 ? (*m1) : -(*m1);
   int_t un2 = m2 > 0 ? m2 : -m2;
   int_t un = un1 + un2;
   int_t sgn1 = (*m1) > 0 ? 1 : -1;
   int_t sgn2 = m2 > 0 ? 1 : -1;
   TMP_INIT;

   TMP_START;
   nn_t t1 = (nn_t) TMP_ALLOC(un*sizeof(uint_t));
   nn_t t2 = (nn_t) TMP_ALLOC(un*sizeof(uint_t));
   nn_t u1 = (nn_t) TMP_ALLOC((un + 1)*sizeof(uint_t));
   nn_t u2 = (nn_t) TMP_ALLOC((un + 1)*sizeof(uint_t));
   nn_t u3 = (nn_t) TMP_ALLOC((un + 1)*sizeof(uint_t));
   nn_t u4 = (nn_t) TMP_ALLOC((un + 1)*sizeof(uint_t));

   if (un2 >= un1)
   {
      nn_mul(t1, M2[0], un2, M1[0], un1);
      nn_mul(t2, M2[1], un2, M1[2], un1);
      u1[un] = nn_add_m(u1, t1, t2, un);

      nn_mul(t1, M2[0], un2, M1[1], un1);
      nn_mul(t2, M2[1], un2, M1[3], un1);
      u2[un] = nn_add_m(u2, t1, t2, un);

      nn_mul(t1, M2[2], un2, M1[0], un1);
      nn_mul(t2, M2[3], un2, M1[2], un1);
      u3[un] = nn_add_m(u3, t1, t2, un);

      nn_mul(t1, M2[2], un2, M1[1], un1);
      nn_mul(t2, M2[3], un2, M1[3], un1);
      u4[un] = nn_add_m(u4, t1, t2, un);
   } else
   {
      nn_mul(t1, M1[0], un1, M2[0], un2);
      nn_mul(t2, M1[2], un1, M2[1], un2);
      u1[un] = nn_add_m(u1, t1, t2, un);

      nn_mul(t1, M1[1], un1, M2[0], un2);
      nn_mul(t2, M1[3], un1, M2[1], un2);
      u2[un] = nn_add_m(u2, t1, t2, un);

      nn_mul(t1, M1[0], un1, M2[2], un2);
      nn_mul(t2, M1[2], un1, M2[3], un2);
      u3[un] = nn_add_m(u3, t1, t2, un);

      nn_mul(t1, M1[1], un1, M2[2], un2);
      nn_mul(t2, M1[3], un1, M2[3], un2);
      u4[un] = nn_add_m(u4, t1, t2, un);
   }

   un1 = nn_normalise(u1, un);
   un1 = CCAS_MAX(un1, nn_normalise(u2, un));
   un1 = CCAS_MAX(un1, nn_normalise(u3, un));
   un1 = CCAS_MAX(un1, nn_normalise(u4, un));

   nn_copyi(M1[0], u1, un1);
   nn_copyi(M1[1], u2, un1);
   nn_copyi(M1[2], u3, un1);
   nn_copyi(M1[3], u4, un1);

   TMP_END;

   (*m1) = (sgn1 ^ sgn2) < 0 ? -un1 : un1; 
}
