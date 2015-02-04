#include "ccas.h"
#include "fft.h"

/*
   We compute s = i1 + i2*zeta^y mod p and t = i1 - i2*zeta^y mod p, where
   multiplication by zeta is equivalent to shifting right by one word.
*/
void butterfly_shr(nn_t s, nn_t t, nn_src_t i1, 
                                   nn_src_t i2, int_t limbs, int_t y)
{
   uint_t cy;

   cy = nn_add_m(s, i1, i2 + y, limbs - y);
   s[limbs] = i1[limbs] - nn_sub_m(s + limbs - y, i1 + limbs - y, i2, y);
   fft_add_1(s + limbs - y, y, cy + i2[limbs]);
   
   cy = -nn_sub_m(t, i1, i2 + y, limbs - y);
   t[limbs] = i1[limbs] + nn_add_m(t + limbs - y, i1 + limbs - y, i2, y);
   fft_add_1(t + limbs - y, y, cy - i2[limbs]);
}

/*
   We compute s = i1 + i2*2^-i mod p and t = i1 - i2*2^-i mod p, where
   2 is a root of unity mod p.
   We accomplish this by breaking into a shift by a given number of words
   plus a shift by a given number of bits.
*/
void ifft_butterfly(nn_t s, nn_t t, nn_t i1,  nn_t i2, int_t limbs, int_t i)
{
   CCAS_ASSERT(limbs >= 0);
   CCAS_ASSERT(i >= 0);
   CCAS_ASSERT(i <= limbs*INT_BITS);
   CCAS_ASSERT(!NN_OVERLAP(s, limbs + 1, i1, limbs + 1));
   CCAS_ASSERT(!NN_OVERLAP(s, limbs + 1, i2, limbs + 1));
   CCAS_ASSERT(!NN_OVERLAP(t, limbs + 1, i1, limbs + 1));
   CCAS_ASSERT(!NN_OVERLAP(t, limbs + 1, i2, limbs + 1));
   CCAS_ASSERT(!NN_OVERLAP(s, limbs + 1, t, limbs + 1));

   int_t words = i / INT_BITS;
   int_t bits  = i % INT_BITS;

   fft_div_2exp(i2, i2, limbs, bits);
   butterfly_shr(s, t, i1, i2, limbs, words);
}

