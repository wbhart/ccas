#include "ccas.h"
#include "Poly.h"
#include "parent.h"

void PolyRing(parent_t parent, parent_t base, const char * var)
{
   parent->tag = POLY_T;
   parent->size = sizeof(Poly_struct);
   parent->base = base;
   parent->init = (void (*)(void *, parent_t)) poly_init;
   parent->clear = (void (*)(void *)) poly_clear;
   parent->zero = (void (*)(void *)) poly_zero;
   parent->iszero = (int (*)(void *)) poly_iszero;
   parent->set = (void (*)(void *, void *)) poly_set;
   parent->neg = (void (*)(void *, void *)) poly_neg;
   parent->add = (void (*)(void *, void *, void *)) poly_add;
   parent->sub = (void (*)(void *, void *, void *)) poly_sub;
   parent->mul = (void (*)(void *, void *, void *)) poly_mul;
   parent->getstr = (char * (*)(void *)) poly_getstr;
   parent->aux = (void *) var;
}

