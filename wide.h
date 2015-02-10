#include "ccas.h"

/**
   Set lo to the product of x and y, plus cy and return any carry.
*/
static inline
uint_t widemul(uint_t * lo, uint_t x, uint_t y, uint_t cy)
{
   wide_t r = (wide_t) x * (wide_t) y + (wide_t) cy;
   (*lo) = (uint_t) r;

   return (uint_t) (r >> INT_BITS);
}

/**
   Add the product of x and y, plus cy, to lo and return any carry.
*/
static inline
uint_t wideaddmul(uint_t * lo, uint_t x, uint_t y, uint_t cy)
{
   wide_t r = (wide_t) (*lo) + (wide_t) x * (wide_t) y + (wide_t) cy;
   (*lo) = (uint_t) r;

   return (uint_t) (r >> INT_BITS);
}

/**
   Subtract the product of x and y, plus bw, from lo and return any borrow.
*/
static inline
uint_t widesubmul(uint_t * lo, uint_t x, uint_t y, uint_t bw)
{
   wide_t r = (wide_t) (*lo) - (wide_t) x * (wide_t) y - (wide_t) bw;
   (*lo) = (uint_t) r;

   return -(uint_t) (r >> INT_BITS);
}

/**
   Set lo to sum of x and y and return any carry.
*/
static inline
uint_t wideadd(uint_t * lo, uint_t x, uint_t y)
{
   wide_t r = (wide_t) x + (wide_t) y;
   (*lo) = (uint_t) r;

   return (uint_t) (r >> INT_BITS);
}

/**
   Set lo to the difference of x and y and return any borrow.
*/
static inline
uint_t widesub(uint_t * lo, uint_t x, uint_t y)
{
   wide_t r = (wide_t) x - (wide_t) y;
   (*lo) = (uint_t) r;

   return -(uint_t) (r >> INT_BITS);
}

/**
   Set lo to sum of x and y + cy, and return any carry.
*/
static inline
uint_t wideadc(uint_t * lo, uint_t x, uint_t y, uint_t cy)
{
   wide_t r = (wide_t) x + (wide_t) y + (wide_t) cy;
   (*lo) = (uint_t) r;

   return (uint_t) (r >> INT_BITS);
}

/**
   Set lo to difference of x and y + bw, and return any borrow, either 0 or 1.
*/
static inline
uint_t widesbb(uint_t * lo, uint_t x, uint_t y, uint_t bw)
{
   wide_t r = (wide_t) x - (wide_t) y - (wide_t) bw;
   (*lo) = (uint_t) r;

   return -(uint_t) (r >> INT_BITS);
}

/**
   Shift x left by the given number of bits and add cy. Set lo to the result
   and return any carry.
*/
static inline
uint_t wideshl(uint_t * lo, uint_t x, int_t bits, uint_t cy)
{
   wide_t r = (((wide_t) x) << bits) + (wide_t) cy;
   (*lo) = (uint_t) r;

   return (uint_t) (r >> INT_BITS);
}

/**
   Shift x right by the given number of bits. Set lo to the result plus cy
   and return any bits shifted out at the right.
*/
static inline
uint_t wideshr(uint_t * lo, uint_t x, int_t bits, uint_t cy)
{
   int_t bits2 = INT_BITS - bits;
   wide_t r = (((wide_t) x) << bits2);
   (*lo) = (uint_t) (r >> INT_BITS) + cy;

   return (uint_t) r;
}

/**
   Set lo to the complement of x, plus cy. Return any carry.
*/
static inline
uint_t cmc(uint_t * lo, uint_t x, uint_t cy)
{
   wide_t r = (wide_t) ~x + (wide_t) cy;
   (*lo) = (uint_t) r;

   return (uint_t) (r >> INT_BITS);
}

/**
   Set q to (cy, x) / d and return any remainder.
   Assumes cy < d and d != 0.
*/
static inline
uint_t widedivrem(uint_t * q, uint_t cy, uint_t x, uint_t d)
{
   CCAS_ASSERT(cy < d);
   CCAS_ASSERT(d != 0);

   wide_t r = (((wide_t) cy) << INT_BITS) + x;
   (*q) = (uint_t) (r / d);
   
   return (uint_t) (r % d);
}

/**
   Precomputes an inverse of d. The value of the inverse is
   2^(2*INT_BITS) / (d + 1) - 2^INT_BITS. 
   We assume d is normalised, i.e. the most significant bit
   of d is set.
*/
static inline
uint_t preinvert1(uint_t d)
{
   CCAS_ASSERT(d != 0); 
   
   wide_t r = (((wide_t) -(++d)) << INT_BITS);

   return d == 0 ? 0 : (uint_t) (r / d);
}

/**
   Return the approximate quotient of (hi, lo) by d. The result is either
   correct or too small.
   Assumes d is normalised, i.e. most significant bit set, hi < d
   and dinv is a precomputed inverse of d supplied by preinvert1().

   The algorithm computes the most significant word of 
   (hi, lo)*(2^INT_BITS + dinv).
*/
static inline
uint_t widedivappr_pi1(uint_t hi, uint_t lo, uint_t d, uint_t dinv)
{
   CCAS_ASSERT(hi < d);
   CCAS_ASSERT(NORMALISED(d));

   const wide_t u = (((wide_t) hi) << INT_BITS) + (wide_t) lo;
   wide_t r, q1 = (wide_t) hi * (wide_t) dinv + u;
   const wide_t q0 = (wide_t) lo * (wide_t) dinv;

   uint_t qd = (uint_t) ((q1 + (q0 >> INT_BITS)) >> INT_BITS);
	
   return qd;
}

/**
   Set q to the quotient of (hi, lo) by d and returns the remainder.
   Assumes d is normalised, i.e. most significant bit set, hi < d
   and dinv is a precomputed inverse of d supplied by preinvert1().

   The algorithm computes an approximate quotient which may be too
   small, computes the remainder and adjusts until the remainder is
   in [0, d).
*/
static inline
uint_t widedivrem_pi1(uint_t * q, uint_t hi, uint_t lo, uint_t d, uint_t dinv)
{
   const wide_t u = (((wide_t) hi) << INT_BITS) + (wide_t) lo;
   (*q) = widedivappr_pi1(hi, lo, d, dinv);
   wide_t r = u - (wide_t) d * (wide_t) (*q);
      
   while (r >= d)
   { 
      r -= d;
      (*q)++;
   }
  
   return (uint_t) r;
}

