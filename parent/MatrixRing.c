#include "ccas.h"
#include "matrix.h"
#include "parent.h"

void MatrixRing(parent_t parent, parent_t base, const char * var)
{
   parent->tag = MAT_T;
   parent->size = sizeof(Mat_struct);
   parent->base = base;
   parent->init = (void (*)(void *, parent_t)) mat_init;
   parent->clear = (void (*)(void *)) mat_clear;
   //parent->zero = (void (*)(void *)) mat_zero;
   //parent->iszero = (int (*)(void *)) mat_iszero;
   //parent->seti = (void (*)(void *, int_t)) mat_seti;
   //parent->setz = (void (*)(void *, zz_t)) mat_setz;
   parent->set = (void (*)(void *, void *)) mat_set;
   //parent->neg = (void (*)(void *, void *)) mat_neg;
   //parent->addi = (void (*)(void *, void *, int_t)) mat_addi;
   //parent->addz = (void (*)(void *, void *, zz_t)) mat_addz;
   //parent->add = (void (*)(void *, void *, void *)) mat_add;
   //parent->sub = (void (*)(void *, void *, void *)) mat_sub;
   //parent->muli = (void (*)(void *, void *, int_t)) mat_muli;
   //parent->mulz = (void (*)(void *, void *, zz_t)) mat_mulz;
   //parent->mul = (void (*)(void *, void *, void *)) mat_mul;
   //parent->getstr = (char * (*)(void *)) mat_getstr;
   parent->aux = (void *) var;
}

