#include "nn.h"

#ifndef ZZ_H
#define ZZ_H

typedef struct
{
   nn_t n;
   int_t size;
   int_t alloc;
} zz_struct;

typedef zz_struct zz_t[1];
typedef zz_struct * zz_ptr;
typedef const zz_struct * zz_srcptr;

/**
   Efficiently swap zz_ptrs by copying pointers.
*/
#define zz_ptrswap(a, b)        \
   do {                         \
      zz_ptr __t = (zz_ptr)(a); \
      (a) = (b);                \
      (b) = __t;                \
   } while (0)

/**
   Given two zz_t's, a and b and two corresponding absolute values of
   sizes an and bn, order the zz_t's and sizes so that an >= bn.
*/
#define zz_order(a, an, b, bn) \
   do {                        \
      if ((an) < (bn)) {           \
         int_t __t = (an);     \
         (an) = (bn);          \
         (bn) = __t;           \
         zz_ptrswap(a, b);     \
      }                        \
   } while (0)

/**********************************************************************
 
    Memory management

**********************************************************************/

/**
   Initialise a zz_t for use.
*/
void zz_init(zz_ptr r);

/**
   Free any memory resources used by the given zz_t.
*/
void zz_clear(zz_ptr r);

/**
   Make space in the zz_t r for an integer whose absolute value is m words.
   We require m >= 0.
*/
void zz_fit(zz_ptr r, int_t m);

/**
   Normalise the zz_t r so that either r = 0 or the leading word of
   {r.d, |r.size|} is nonzero.
*/
void zz_normalise(zz_ptr r);

/**********************************************************************
 
    Basic manipulation

**********************************************************************/

/**
   Set the zz_t r to the signed word c.
*/
void zz_seti(zz_ptr r, int_t c);

/**
   Set the zz_t r to the value of the zz_t a.
*/
void zz_set(zz_ptr r, zz_srcptr a);

/**
   Efficiently swap zz_t's by swapping the struct internals.
*/
static inline
void zz_swap(zz_ptr a, zz_ptr b)
{
   nn_swap(a->n, b->n);

   int_t t = a->size;
   a->size = b->size;
   b->size = t;

   t = a->alloc;
   a->alloc = b->alloc;
   b->alloc = t;
}

/**
   Set r to -a.
*/
void zz_neg(zz_ptr r, zz_srcptr a);

/**
   Set the zz_t r to zero.
*/
static inline
void zz_zero(zz_ptr r)
{
   r->size = 0;
}

/**********************************************************************

    Comparison

**********************************************************************/

/**
   Return 1 or 0 depending whether the zz_t a is equal to or not equal
   to the zz_t b.
*/
int zz_equal(zz_srcptr a, zz_srcptr b);

/**
   Return 1 or 0 depending whether the zz_t a is equal to or not equal
   to the signed word c.
*/
int zz_equali(zz_srcptr a, int_t c);

/**
   Return -1, 0, 1 depending whether the zz_r a is less than, equal to
   or greater than the zz_t b.
*/
int zz_cmp(zz_srcptr a, zz_srcptr b);

/**
   Return -1, 0, 1 depending whether the zz_r a is less than, equal to
   or greater than the int_t c.
*/
int zz_cmpi(zz_srcptr a, int_t c);

/**
   Return -1, 0, 1 depending whether the absolute value of the zz_r a
   is less than, equal to or greater than the absolute value of the
   zz_t b.
*/
int zz_cmpabs(zz_srcptr a, zz_srcptr b);

/**
   Return 1 or 0 depending whether the zz_r a is equal to or not equal
   to zero.
*/
static inline
int zz_iszero(zz_srcptr a)
{
   return a->size == 0;
}

/**********************************************************************
 
    Random

**********************************************************************/

/**
   Set the zz_t a to a uniformly random integer whose absolute value
   has exactly |bits| bits (i.e. with the most significant bit set).
   If bits is negative the value will uniformly randomly be made
   negative with probability 1/2.
*/
void zz_randbits(zz_ptr a, rand_t state, int_t bits);

/**********************************************************************

    Basic arithmetic

**********************************************************************/

/**
   Set r to a + b.
*/
void zz_add(zz_ptr r, zz_srcptr a, zz_srcptr b);

/**
   Set r to a - b.
*/
void zz_sub(zz_ptr r, zz_srcptr a, zz_srcptr b);

/**
   Set r to a + c where c is a signed word.
*/
void zz_addi(zz_ptr r, zz_srcptr a, int_t c);

/**
   Set r to a - c where c is a signed word.
*/
void zz_subi(zz_ptr r, zz_srcptr a, int_t c);

/**
   Set r to a*b.
*/
void zz_mul(zz_ptr r, zz_srcptr a, zz_srcptr b);

/**
   Set r to a*c where c is a signed word.
*/
void zz_muli(zz_ptr r, zz_srcptr a, int_t c);

/**
   Set r to a*2^s. We require s >= 0.
*/
void zz_mul_2exp(zz_ptr r, zz_srcptr a, int_t s);

/**********************************************************************

    Division

**********************************************************************/

/**
   Set r to a/2^s and discard the remainder. Rounding is towards
   zero. We require s >= 0.
*/
void zz_div_2exp(zz_ptr r, zz_srcptr a, int_t s);

/**
   Set q to the quotient of a by the signed word b and return the
   remainder. Rounding is toward zero.
*/
int_t zz_divremi(zz_ptr q, zz_srcptr a, int_t b);

/**
   Set q, r to the quotient and remainder of a divided by b, with
   rounding towards zero.
*/
void zz_divrem(zz_ptr q, zz_ptr r, zz_srcptr a, zz_srcptr b);

/**
   Set q to the quotient a divided by b, with rounding towards zero.
*/
void zz_div(zz_ptr q, zz_srcptr a, zz_srcptr b);

/**********************************************************************

    GCD

**********************************************************************/

/**
   Set g to the greatest common divisor of a and b.
*/
void zz_gcd(zz_ptr g, zz_srcptr a, zz_srcptr b);

/**
   Set g to the greatest common divisor of a and b and find s and t
   such that as + bt = g.
*/
void zz_xgcd(zz_ptr g, zz_ptr s, zz_ptr t, zz_srcptr a, zz_srcptr b);

/**********************************************************************

    String I/O

**********************************************************************/

/**
   Return a decimal string representation of a. The user is
   responsible for freeing the string when finished.
*/
char * zz_getstr(zz_srcptr a);

/**
   Set a to the value of the decimal string str, which may have an
   optional minus sign (without intervening whitespace). The number
   of characters read, including for the minus sign, is returned.
*/
int_t zz_setstr(zz_ptr a, const char * str);

/**
   Print a decimal representation of a to the given stream. No
   newline is printed.
*/
static inline
void zz_fprint(FILE * file, zz_srcptr a)
{
   char * str = zz_getstr(a);
   
   fprintf(file, "%s", str);

   free(str);
}

/**
   Print a decimal representation of a to stdout. No newline is
   printed.
*/
static inline
void zz_print(zz_srcptr a)
{
   zz_fprint(stdout, a);
}

/**
   Read an integer with optional minus sign from the given stream.
   If there is not enough memory, the function aborts.
*/
void zz_fread(zz_ptr a, FILE * file);

#endif

