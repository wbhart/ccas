#include <stdio.h>
#include "parent.h"

typedef struct
{
   void * coeffs;
   int_t length;
   int_t alloc;
   parent_struct * parent;
} Poly_struct;

typedef Poly_struct Poly_t[1];

/*****************************************************************************

   Memory management

*****************************************************************************/

/**
   Initialise a generic polynomial for use. It will have the given
   parent, which should be obtained from the PolyRing constructor.
*/
void poly_init(Poly_t pol, parent_t parent);

/**
   Clean up by freeing any memory used by the given generic polynomial.
*/
void poly_clear(Poly_t pol);

/**
   By reallocating if necessary, ensure that the given polynomial has
   space for len coefficients.
*/
void poly_fit(Poly_t pol, int_t len);

/**
   Normalise the given generic polynomial, i.e. ensure that the
   polynomial is either zero or has nonzero leading coefficient.
*/
static inline
void poly_normalise(Poly_t pol)
{
   int (* iszero)(void *) = pol->parent->base->iszero;
   int_t size = pol->parent->base->size;
   
   while (pol->length > 0 && iszero(pol->coeffs + (pol->length - 1)*size))
      pol->length--;
}

/*****************************************************************************

   Swapping

*****************************************************************************/

/**
   Efficiently swap two polynomials by swapping the contenst of their
   structs, rather than copying the coefficient data.
*/
static inline
void poly_swap(Poly_t a, Poly_t b)
{
   if (a != b)
   {
      Poly_struct t = (*a);
     (*a) = (*b);
     (*b) = t;
   }
}

/*****************************************************************************

   Copying and negation

*****************************************************************************/

/**
   Set the polynomial a to equal the polynomial b.
*/
void poly_set(Poly_t a, Poly_t b);

/**
   Set the polynomial a to -b.
*/
void poly_neg(Poly_t a, Poly_t b);

/*****************************************************************************

   Assignment

*****************************************************************************/

/**
  Set the polynomial a to the constant polynomial with value n.
*/
void poly_seti(Poly_t a, int_t n);

/**
  Set the polynomial a to the constant polynomial with value n.
*/
void poly_setz(Poly_t a, zz_t n);

/**
  Set the polynomial a to the zero polynomial.
*/
static inline
void poly_zero(Poly_t a)
{
  a->length = 0;
}

/*****************************************************************************

   Comparison

*****************************************************************************/

/**
   Return 1 if the polynomial a is the zero polynomial, otherwise
   return 0.
*/
static inline
int poly_iszero(Poly_t a)
{
  return a->length == 0;
}

/*****************************************************************************

   Coefficient manipulation

*****************************************************************************/

/**
   Return a pointer to coefficient n of the given polynomial, where
   coefficients are numbered from 0.
*/
static inline
void * poly_coeff_ptr(Poly_t pol, int_t n)
{
   int_t size = pol->parent->base->size;

   return pol->coeffs + n*size;
}

/**
   Set coefficient n of the given polynomial to the integer c.
*/
void poly_coeff_seti(Poly_t pol, int_t n, int_t c);

/**
   Set coefficient n of the given polynomial to the multiprecision
   integer c.
*/
void poly_coeff_setz(Poly_t pol, int_t n, zz_t c);

/**
   Set coefficient n of the given polynomial to the value of the
   generic pointed to by coeff.
*/
void poly_coeff_set(Poly_t pol, int_t n, void * coeff);

/*****************************************************************************

   Basic arithmetic

*****************************************************************************/

/**
   Set the polynomial a to b + c.
*/
void poly_add(Poly_t a, Poly_t b, Poly_t c);

/**
   Set the polynomial a to b - c.
*/
void poly_sub(Poly_t a, Poly_t b, Poly_t c);

/**
   Set the polynomial a to b*c.
*/
void poly_mul(Poly_t a, Poly_t b, Poly_t c);

/*****************************************************************************

   Ad hoc arithmetic

*****************************************************************************/

/**
   Set the polynomial a to b + c, where c is an integer.
*/
void poly_addi(Poly_t a, Poly_t b, int_t c);

/**
   Set the polynomial a to b + c, where c is an integer.
*/
void poly_addz(Poly_t a, Poly_t b, zz_t c);

/**
   Set the polynomial a to b*c, where c is an integer.
*/
void poly_muli(Poly_t a, Poly_t b, int_t c);

/**
   Set the polynomial a to b*c, where c is a multiprecision integer.
*/
void poly_mulz(Poly_t a, Poly_t b, zz_t c);

/*****************************************************************************

   Powering

*****************************************************************************/

/**
   Set the polynomial a to b^n, where n is nonnegative.
*/
void poly_pow(Poly_t a, Poly_t b, int_t n);

/*****************************************************************************

   String I/O

*****************************************************************************/

/**
   Return a string representation of the given polynomial, e.g. for
   printing. An example would be the string
   "(3*x^2 + 2*x - 1)*y^2 + (x + 1)*y - x + 1"
   The variables used for printing are specified when creating the
   associated parent object with the PolyRing constructor.
*/
char * poly_getstr(Poly_t a);

/**
   Print the string representation of the given polynomial to
   standard output.
*/
static inline
void poly_print(Poly_t a)
{
   char * str = poly_getstr(a);

   printf("%s", str);

   free(str);
}

