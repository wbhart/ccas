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

void poly_init(Poly_t pol, parent_t parent);

void poly_clear(Poly_t pol);

void poly_fit(Poly_t pol, int_t len);

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

void poly_set(Poly_t a, Poly_t b);

void poly_neg(Poly_t a, Poly_t b);

/*****************************************************************************

   Assignment

*****************************************************************************/

void poly_seti(Poly_t a, int_t n);

void poly_setz(Poly_t a, zz_t c);

static inline
void poly_zero(Poly_t a)
{
  a->length = 0;
}

/*****************************************************************************

   Comparison

*****************************************************************************/

static inline
int poly_iszero(Poly_t a)
{
  return a->length == 0;
}

/*****************************************************************************

   Coefficient manipulation

*****************************************************************************/

static inline
void * poly_coeff_ptr(Poly_t pol, int_t n)
{
   int_t size = pol->parent->base->size;

   return pol->coeffs + n*size;
}

void poly_coeff_seti(Poly_t pol, int_t n, int_t c);

void poly_coeff_setz(Poly_t pol, int_t n, zz_t c);

void poly_coeff_set(Poly_t pol, int_t n, void * coeff);

/*****************************************************************************

   Basic arithmetic

*****************************************************************************/

void poly_add(Poly_t a, Poly_t b, Poly_t c);

void poly_sub(Poly_t a, Poly_t b, Poly_t c);

void poly_mul(Poly_t a, Poly_t b, Poly_t c);

/*****************************************************************************

   Ad hoc arithmetic

*****************************************************************************/

void poly_addi(Poly_t a, Poly_t b, int_t c);

void poly_addz(Poly_t a, Poly_t b, zz_t c);

void poly_muli(Poly_t a, Poly_t b, int_t c);

void poly_mulz(Poly_t a, Poly_t b, zz_t c);

/*****************************************************************************

   Powering

*****************************************************************************/

void poly_pow(Poly_t a, Poly_t b, int_t n);

/*****************************************************************************

   String I/O

*****************************************************************************/

char * poly_getstr(Poly_t a);

static inline
void poly_print(Poly_t a)
{
   char * str = poly_getstr(a);

   printf("%s", str);

   free(str);
}

/*****************************************************************************

   PolyRing constructor

*****************************************************************************/

void PolyRing(parent_t parent, parent_t base, const char * var);

