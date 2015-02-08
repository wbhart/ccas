#include "ccas.h"
#include "nn.h"

void nn_ngcd_mat_identity(nn_t * M, int_t * mn)
{
   M[0][0] = 1;
   M[1][0] = 0;
   M[2][0] = 0;
   M[3][0] = 1;

   (*mn) = 1;
}
