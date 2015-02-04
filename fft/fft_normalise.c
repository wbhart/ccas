#include "ccas.h"
#include "fft.h"

void fft_normalise(nn_t t, int_t limbs)
{
   CCAS_ASSERT(limbs >= 0);

   int_t hi = t[limbs];
   
   if (hi != 0)
   {
      t[limbs] = 0;
      fft_add_1(t, limbs, -hi);

      /* hi will now be in [-1,1] */
      if ((hi = t[limbs]))
      {
         t[limbs] = 0;
         fft_add_1(t, limbs, -hi);

         /* if we now have -1 (very unlikely) */
         if (-t[limbs] == 1)
         {
            t[limbs] = 0;
            fft_add_1(t, limbs, 1);            
         }
      }
   }
}

