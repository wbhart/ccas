#include "ccas.h"
#include "nn.h"

/**
   Set r to r + c where c is a signed value.
   We require c to be small in absolute value.
*/
static inline
void fft_add_1(nn_t r, int_t limbs, int_t c)
{
   CCAS_ASSERT(limbs >= 0);

   const uint_t sum = r[0] + c;

   /* check if adding c would cause a carry to propagate */
   if ((int_t)(sum ^ r[0]) >= 0)
      r[0] = sum;
   else
   {
      if (c >= 0) nn_add_1(r, r, limbs + 1, c);
      else nn_sub_1(r, r, limbs + 1, -c);
   }
}

/**
   Set t to i1*2^i mod p. Both i1 and t consist of the given number of limbs
   plus an overflow limb.
   We require 0 <= i < INT_BITS.
*/
void fft_mul_2exp(nn_t t, nn_src_t i1, int_t limbs, int_t i);

/**
   Set t to i1/2^i mod p. Both i1 and t consist of the given number of limbs
   plus an overflow limb.
   We require 0 <= i < INT_BITS.
*/
void fft_div_2exp(nn_t t, nn_src_t i1, int_t limbs, int_t i);

/**
   Normalise an fft coefficient of the given number of limbs with an overflow
   limb modulo p = 2^(limbs*INT_BITS) + 1 so that it is in the range [0, p).
*/
void fft_normalise(nn_t t, int_t limbs);

/**
   Multiply the fft coefficients ii and jj of the given number of limbs with
   extra overflow limbs, modulo p = 2^(limbs*INT_BITS) + 1. It is assumed
   that ii and jj are normalised beforehand, i.e. in the range [0, p).
   Aliasing is permitted.
*/
void fft_mulmod(nn_t rr, nn_src_t ii, nn_src_t jj, int_t limbs);

/**
   Set s = i1 + i2 mod p and t = (i1 - i2)*2^i mod p, where
   2 is a root of unity mod p.
   We require i <= limbs*INT_BITS. Here i1, i2, s, t consist of the
   given number of limbs plus an overflow limb.
   No aliasing of s or t with i1 or i2 is permitted, no between s and t.
*/
void fft_butterfly(nn_t s, nn_t t, nn_t i1, nn_t i2, int_t limbs, int_t i);

/**
   Set s = i1 + i2*2^-i mod p and t = i1 - i2*2^-i mod p, where
   2 is a root of unity mod p.
   We require i <= limbs*INT_BITS. Here i1, i2, s, t consist of the
   given number of limbs plus an overflow limb.
   No aliasing of s or t with i1 or i2 is permitted, no between s and t.
*/
void ifft_butterfly(nn_t s, nn_t t, nn_t i1, nn_t i2, int_t limbs, int_t i);

/**
   Perform an inplace radix 2 fft on the vector of fft coefficients at
   ii. Each is (n*w)/INT_BITS limbs in size, plus an overflow limb.
   Shifting left by w bits is the multiplication by the appropriate
   root of unity in Z/pZ where p = 2^(w*n) + 1, for this length 2*n
   convolution.
   We require w*n to be divisible by INT_BITS.
*/
void fft_radix2(nn_t * ii, int_t n, int_t w);

/**
   Perform an inplace radix 2 ifft on the vector of fft coefficients at
   ii. Each is (n*w)/INT_BITS limbs in size, plus an overflow limb.
   Shifting right by w bits is the division by the appropriate
   root of unity in Z/pZ where p = 2^(w*n) + 1, for this length 2*n
   convolution.
   We require w*n to be divisible by INT_BITS.
*/
void ifft_radix2(nn_t * ii, int_t n, int_t w);

/**
   Perform an fft convolution of the vector of fft coefficients at ii
   of length 2*n with that at jj. Each of the coefficients is the given
   number of limbs in length, plus an overflow limb. The result is stored
   in the vector at ii.
   We require n to be a power of 2 and for limbs*INT_BITS to be divisible
   by n.
*/
void fft_convolution(nn_t * ii, nn_t * jj, int_t n, int_t limbs);

/**
   Split an nn_t a of m words into n fft coefficients, taking coeff_limbs
   words of a at a time, where the fft coefficients have output_limbs
   words plus an overflow word.
*/
void fft_split(nn_t * fft, int_t n, nn_src_t a,
                           int_t m, int_t coeff_limbs, int_t limbs);

/**
   Combine the n fft coefficients, each of the given number of limbs plus
   an overflow limb, into a integer of m words. Combination happens by
   treating fft as a polynomial and evaluating it at the value
   2^(coeff_limbs*INT_BITS).
   We require limbs >= coeff_limbs.
*/
void fft_combine(nn_t a, int_t m, nn_t * fft, int_t n,
                                    int_t coeff_limbs, int_t limbs);
