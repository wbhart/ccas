#include <stdio.h>
#include "rand.h"

#define DIVREM_DIVCONQUER_THRESHOLD 30
#define DIVREM_NEWTON_THRESHOLD 1000
#define MUL_KARATSUBA_THRESHOLD 30
#define MUL_FFT_THRESHOLD 1000
#define NGCD_THRESHOLD 100

typedef uint_t * nn_t;
typedef const uint_t * nn_src_t;

#define NN_OVERLAP(a, m, b, n) \
   ((b) > (a) - (n) && (b) < (a) + (m))

/**
   Swap the nn_t's a and b by swapping pointers.
*/
#define nn_swap(a, b) \
   do { \
      nn_t __t = (a); \
      (a) = (b); \
      (b) = __t; \
   } while (0)

/****************************************************************************

   Memory management
   
*****************************************************************************/

/**
   Allocate an nn_t with space for m words.
*/
static inline
uint_t * nn_alloc(int_t m)
{
   CCAS_ASSERT (m >= 0);
   
   return (uint_t *) malloc(m*sizeof(uint_t));
}

/**
   Free the memory allocated for an nn_t by nn_alloc.
*/
static inline
void nn_free(uint_t * a)
{
   free(a);
}

/****************************************************************************

   Linear algorithms
   
*****************************************************************************/

/**
   Set {a, m} to {b, m}. We require a coincides with b or occurs before it
   in memory.
*/
void nn_copyi(nn_t a, nn_src_t b, int_t m);

/**
   Set {a, m} to {b, m}. We require a coincides with b or occurs after it
   in memory.
*/
void nn_copyd(nn_t a, nn_src_t b, int_t m);

/**
   Set {a, m} to zero.
*/
void nn_zero(nn_t a, int_t m);

/**
   Given {a, m} possibly with leading zeros, return the normalised length
   m' such that {a, m'} does not have leading zeros.
*/
int_t nn_normalise(nn_src_t a, int_t m);

/**
   Set {a, m} to the sum of {b, m} and {c, m} and return any carry.
*/
uint_t nn_add_m(nn_t a, nn_src_t b, nn_src_t c, int_t m);

/**
   Set {a, m} to the difference of {b, m} and {c, m} and return any borrow.
*/
uint_t nn_sub_m(nn_t a, nn_src_t b, nn_src_t c, int_t m);

/**
   Set {a, m} to the sum of {b, m} and cy and return any carry.
*/
uint_t nn_add_1(nn_t a, nn_src_t b, int_t m, uint_t cy);

/**
   Set {a, m} to the difference of {b, m} and bw and return any borrow.
*/
uint_t nn_sub_1(nn_t a, nn_src_t b, int_t m, uint_t bw);

/**
   Set {a, m} to the sum of {b, m} and {c, n} and return any carry.
   Assumes m >= n.
*/
static inline
uint_t nn_add(nn_t a, nn_src_t b, int_t m, nn_src_t c, int_t n)
{
   CCAS_ASSERT(a == b || !NN_OVERLAP(a, m, b, m));
   CCAS_ASSERT(a == c || !NN_OVERLAP(a, m, c, n));
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n >= 0);

   uint_t cy = nn_add_m(a, b, c, n);

   return nn_add_1(a + n, b + n, m - n, cy);
}

/**
   Set {a, m} to the difference of {b, m} and {c, n} and return any borrow.
   Assumes m >= n.
*/
static inline
uint_t nn_sub(nn_t a, nn_src_t b, int_t m, nn_src_t c, int_t n)
{
   CCAS_ASSERT(a == b || !NN_OVERLAP(a, m, b, m));
   CCAS_ASSERT(a == c || !NN_OVERLAP(a, m, c, n));
   CCAS_ASSERT(m >= n);
   CCAS_ASSERT(n >= 0);
   
   uint_t bw = nn_sub_m(a, b, c, n);

   return nn_sub_1(a + n, b + n, m - n, bw);
}

/**
   Return 1, -1 or 0 depending whether {a, m} is greater than, less than
   or equal {b, m}, respectively.
*/
int nn_cmp(nn_t a, nn_src_t b, int_t m);

/**
   Set {a, m} to -{b, m} and return 1 if {a, m} was zero, otherwise return 0.
*/
uint_t nn_neg(nn_t a, nn_src_t b, int_t m);

/**
   Set {a, m} to {b, m} times c and return any carry.
*/
uint_t nn_mul_1(nn_t a, nn_src_t b, int_t m, uint_t c);

/**
   Set {a, m} to {a, m} plus {b, m} times c and return any carry.
*/
uint_t nn_addmul_1(nn_t a, nn_src_t b, int_t m, uint_t c);

/**
   Set {a, m} to {a, m} - {b, m} times c and return any borrow.
*/
uint_t nn_submul_1(nn_t a, nn_src_t b, int_t m, uint_t c);

/**
   Set {q, m} to cy, {a, m} / d and return the remainder.
   Requires cy < d.
*/
uint_t nn_divrem_1(nn_t q, uint_t cy, nn_src_t a, int_t m, uint_t d);

/**
   Set {q, m} to cy, {a, m} / d and return the remainder.
   Requires cy < d and d to be normalised, with dinv a precomputed inverse
   of d supplied by preinvert1().
*/ 
uint_t nn_divrem_1_pi1(nn_t q, uint_t cy, nn_src_t a, int_t m,
                                                   uint_t d, uint_t dinv);

/**
   Set {a, m} to {b, m} shifted left by the given number of bits, and
   return any bits shifted out at the left.
   We require 0 <= bits < INT_BITS.
*/
uint_t nn_shl(nn_t a, nn_src_t b, int_t m, int_t bits);

/**
   Set {a, m} to {b, m} shifted right by the given number of bits, and
   return any bits shifted out at the right.
   We require 0 <= bits < INT_BITS.
*/
uint_t nn_shr(nn_t a, nn_src_t b, int_t m, int_t bits);

/****************************************************************************

   Randomisation
   
*****************************************************************************/

/**
   Set {a, m} to a uniformly random value in the range [0, 2^bits),
   but with the high bit of the random value set so that the number
   has exactly the given number of bits. The  m*INT_BITS - bits
   most significant bits of {a, m} are set to zero.
   We require that 0 <= bits <= m*INT_BITS. 
*/ 
void nn_randbits(nn_t a, int_t m, rand_t state, int_t bits);

/****************************************************************************

   String I/O
   
*****************************************************************************/

/** 
   Return a string giving the decimal expansion of {a, m}.
   The user is responsible for freeing the string after use.
*/
char * nn_getstr(nn_t a, int_t m);

/**
   Set {a, len} to the integer represented by str in base 10.
   The number of digits in str is returned.
*/
int_t nn_setstr(nn_t a, int_t * len, const char * str);

/**
   Print the integer {a, m} to stdout.
*/
static inline
void nn_print(nn_t a, int_t m)
{
   CCAS_ASSERT(m >= 0);
   
   char * str = nn_getstr(a, m);
   
   printf("%s", str);
   
   free(str);
}

/****************************************************************************

   Quadratic algorithms
   
*****************************************************************************/

/**
   Set {r, m + n} to {a, m} times {b, n}. Note that the top word of
   {r, m + n} may be zero.
   We require m >= n >= 0. No aliasing of r with a or b is permitted.
*/
void nn_mul_classical(nn_t r, nn_src_t a, int_t m, nn_src_t b, int_t n);

/**
   Set {q, m - n + 1} to the quotient of cy, {a, m} by {d, n} and leave the
   remainder in {a, n}.
   We require m >= n > 0 and that d is normalised, i.e. most significant
   bit of the most significant word is set. We also require that the top n
   words of cy, {a, m} are less than {d, n}. No aliasing is permitted between
   q, a and d.
*/
void nn_divrem_classical_pi1(nn_t q, uint_t cy, nn_t a, int_t m,
                                          nn_src_t d, int_t n, uint_t dinv);

/**
   Set g to the greatest common divisor of {a, m} and {b, n}. The length
   of g in words is returned by the function. We require g to have space
   for n words.
   We require m >= n > 0.
*/
int_t nn_gcd_euclidean(nn_t g, nn_t a, int_t m, nn_t b, int_t n);

/**
   Set g to the greatest common divisor of {a, m} and {b, n}. The length
   of g in words is returned by the function.
   We set {s, sn} to the cofactor of a in the expression g = a*s + b*t.
   Note that sn may be less than zero.
   We require m >= n > 0. No aliasing of g and s is allowed. We require
   g and s to both have space for n words.
*/
int_t xgcd_euclidean(nn_t g, nn_t s, int_t * sn, nn_src_t a, int_t m,
                                                        nn_src_t b, int_t n);

/****************************************************************************

   Subquadratic algorithms
   
*****************************************************************************/

/**
   Set {p, m + n} to {a, m} times {b, n}.
   We require m >= n >= (m + 1)/2. No aliasing is permitted between p and
   a or b.
*/
void nn_mul_karatsuba(nn_t p, nn_src_t a, int_t m, nn_src_t b, int_t n);

/**
   Compute the quotient {q, m - n + 1} and remainder {a, n} of cy, {a, m} by
   {d, n}. We require {d, n} to be normalised, i.e. most significant bit
   set. We also require 2*n - 1 >= m >= n > 0. Note that the remainder is
   stored in-place in {a, n}. We also require the top n words of cy, {a, m}
   to be less that {d, n}. The word pi1 must be set to a precomputed inverse
   of d[n - 1] supplied by preinvert1(). No aliasing is permitted between q,
   a and d.
*/
void nn_divrem_divconquer_pi1(nn_t q, uint_t cy, nn_t a, int_t m, 
                                             nn_src_t d, int_t n, uint_t pi1);
											
/****************************************************************************

   Asymptotically fast algorithms
   
*****************************************************************************/

/**
   Set {r, m1 + m2} to {a1, m1} times {a2, m2}, using the FFT multiplication
   algorithm.
 */
void nn_mul_fft(nn_t r, nn_src_t a1, int_t m1, nn_src_t a2, int_t m2);

/**
   Compute a floating point approximation to 1/{d, n} using Newton iteration.
   We require D = {d, n} to be normalised, i.e. most significant bit set. We
   also require n > 0. We compute X = B^n + {x, n} such that
   DX < B^2n < D(X + 4). That is, {x, n} will be a newton inverse with implicit
   leading bit. The algorithm is an adapted version of that found in the paper
   "Asymptotically fast division for GMP" by Paul Zimmermann, from here:
   http://www.loria.fr/~zimmerma/papers/invert.pdf
   The algorithm has the same asymptotic complexity as multiplication, i.e.
   computes the inverse in O(M(n)) where M(n) is the bit complexity of 
   multiplication.
   The word pi1 is required to be a precomputed inverse of d[n - 1], supplied
   by preinvert1().
*/
void nn_invert_pi1(nn_t x, nn_src_t d, int_t n, uint_t pi1);

/**
   Compute the quotient {q, m - n + 1} and remainder {a, n} of cy, {a, m} by
   {d, n}. We require {d, n} to be normalised, i.e. most significant bit
   set. We also require 2*n - 1 >= m >= n >= 2. Note that the remainder is
   stored in-place in {a, n}. We also require the top n words of cy, {a, m}
   to be less that {d, n}. The value {dinv, n} must be set to an approximate
   floating point inverse of {d, n}, with implicit leading bit, supplied by
   nn_invert_pi1(). No aliasing is permitted between q, a and d.
*/
void nn_divrem_newton_pi(nn_t q, uint_t cy, nn_t a, int_t m, 
                                              nn_src_t d, int_t n, nn_t dinv);
											  
/**
   Compute a quotient {q, m - n + 1} and not too small remainder {r, n + 1}
   of {a, m} by {b, n}. The remainder is guaranteed to be at least s words
   in length (where s is required to be no less than n). The quotient is
   chosen to be either the usual euclidean quotient or one less.
*/
void nn_ngcd_sdiv(nn_t q, nn_t r, nn_src_t a, int_t m,
                                               nn_src_t b, int_t n, int_t s);
											   
/**
   Return 1, 0 depending whether the number of words in {a, m} - {b, n}
   is greater than s.
   We require m >= n >= s > 0.
*/
int nn_ngcd_sdiv_cmp(nn_src_t a, int_t m, nn_src_t b, int_t n, int_t s);

/**
   Allocate memory for a 2x2 matrix of nn_t's with space for n words each.
*/
nn_t * nn_ngcd_mat_init(int_t n);

/**
   Free memory allocated for matrix M.
*/
void nn_ngcd_mat_clear(nn_t * M);

/**
   Set M to the identity matrix, and set mn to 1, the maximum size of entries
   in M.
*/
void nn_ngcd_mat_identity(nn_t * M, int_t * mn);

/**
   Return 1 if the matrix M, mn is the identity, else return 0.
*/
static inline
int nn_ngcd_mat_is_identity(nn_t * M, int_t mn)
{
   return mn == 1 && M[0][0] == 1 && M[1][0] == 0
                  && M[2][0] == 0 && M[3][0] == 1;
}
/**
   Set the matrix M1 with entries no bigger then |m1| words to M2*M1 where
   M2 is a matrix with entries no bigger than |m2| words. The top right and
   bottom left entry of each matrix are implicitly negated and the signs
   of m1 and m2 denote whether M1 and M2 respectively have been multiplied
   implicitly by that sign.
*/
void nn_ngcd_mat_mul(nn_t * M1, int_t * m1, nn_t * M2, int_t m2);

/**
   Set T = ({a, m}, {b, n}) to T + M(c, d) where c = {a, p1} and d = {b, p1}.
   The number of words of the final value of a is returned. The number of words
   of b will be less than this. Leading words above that point will be zero.
   The top right and bottom left entry of the matrix are implicitly negated and
   the sign of mn denotes whether M has been multiplied implicitly by that
   sign. The entries of M are assumed to have no more than |mn| words.
*/   
int_t nn_ngcd_mat_apply(nn_t a, int_t m, nn_t b, int_t n,
                                                 int_t p1, nn_t * M, int_t mn);
											   
/**
   The matrix M is multiplied by [0, -1; -1, {q, qn}] on the left. The top
   right and bottom left entry of the matrix M are implicitly negated and
   the sign of mn denotes whether M has been multiplied implicitly by that
   sign. The entries of M are assumed to have no more than |mn| words.
*/
void nn_ngcd_mat_update(nn_t * M, int_t * mn, nn_src_t q, int_t qn);

/**
   Half-gcd using the algorithm of Moller:
   http://www.lysator.liu.se/~nisse/archive/S0025-5718-07-02017-0.pdf

   Applies steps of the euclidean algorithm to {a, m} and {b, n} and
   returns a matrix M of determinant 1 such that application of M to
   (a, b)^T would result in the final (a, b). If s = m/2 + 1, the final
   return values a and b will be such that a - b has at most s words. The
   number of words of a is returned by the function, with b being no
   larger than a. A bound on the coefficients in M is given by mn. We
   require that n > s on input. The largest entries of M will not exceed
   m - s < m/2 words.
*/
int_t nn_ngcd(nn_t a, int_t m, nn_t b, int_t n, nn_t * M, int_t * mn);

/**
   Set g to the greatest common divisor of {a, m} and {b, n}. The length
   of g in words is returned by the function. We require g to have space
   for n words.
   We require m >= n > 0.
*/
int_t nn_gcd_ngcd(nn_t g, nn_src_t a, int_t m, nn_src_t b, int_t n);

/****************************************************************************

   Tuned algorithms
   
*****************************************************************************/

/**
   Set {p, 2*m} to {a, m}*{b, m}. No aliasing of p with a or b is
   permitted.
*/
static inline
void nn_mul_m(nn_t p, nn_src_t a, nn_src_t b, int_t m)
{
   CCAS_ASSERT(m >= 0);

   if (m <= MUL_KARATSUBA_THRESHOLD)
      nn_mul_classical(p, a, m, b, m);
   else if (m <= MUL_FFT_THRESHOLD)
      nn_mul_karatsuba(p, a, m, b, m);
   else
      nn_mul_fft(p, a, m, b, m);
}

/**
   Set {p, 2*m} to {a, m}*{b, n}. We require m >= n >= 0. No
   aliasing of p with a or b is permitted.
*/
void nn_mul(nn_t p, nn_src_t a, int_t m, nn_src_t b, int_t n);

/**
   Set {q, m - n + 1} to the quotient of cy, {a, m} by {d, n} and leave the
   remainder in {a, n}.
   We require m >= n > 0 and that d is normalised, i.e. most significant
   bit of the most significant word is set. We also require that the top n
   words of cy, {a, m} are less than {d, n}. No aliasing is permitted between
   q, a and d.
*/
static inline
void nn_divrem_pi1(nn_t q, uint_t cy, nn_t a, int_t m,
                                  nn_src_t d, int_t n, uint_t pi1)
{
   int_t qn = m - n + 1;
   TMP_INIT;
   
   if (qn < DIVREM_DIVCONQUER_THRESHOLD)
	  nn_divrem_classical_pi1(q, cy, a, m, d, n, pi1);
   else if (qn < DIVREM_NEWTON_THRESHOLD || n < DIVREM_NEWTON_THRESHOLD)
      nn_divrem_divconquer_pi1(q, cy, a, m, d, n, pi1);
   else
   {
      TMP_START;
	  nn_t dinv = (nn_t) TMP_ALLOC(n*sizeof(uint_t));
	  nn_invert_pi1(dinv, d, n, pi1);
	  nn_divrem_newton_pi(q, cy, a, m, d, n, dinv);
	  TMP_END;
   }
}

/**
   Compute the quotient {q, m + n - 1} of {m, n} by {d, n}. We require
   m >= n > 0. Note that the high word of the quotient may be zero.
*/
void nn_divrem(nn_t q, nn_t r, nn_src_t a, int_t m, nn_src_t d, int_t n);

/**
   Set {q, m - n + 1} to the quotient of cy, {a, m} by {d, n}.
   We require m >= n > 0 and that d is normalised, i.e. most significant
   bit of the most significant word is set. We also require that the top n
   words of cy, {a, m} are less than {d, n}. No aliasing is permitted between
   q, a and d. The average complexity of the algorithm depends on the size of
   the quotient, not of the size of the inputs a and d.
*/
void nn_div_pi1(nn_t q, uint_t cy, nn_t a, int_t m,
                               nn_src_t d, int_t n, uint_t dinv);
