#include "ccas.h"
#include "parent.h"

parent_t ZZ =
{
  INT_T, sizeof(zz_struct),
  (void (*)(void *, parent_t)) zz_init,
  (void (*)(void *)) zz_clear,
  (void (*)(void *)) zz_zero,
  (int (*)(void *)) zz_iszero,
  (void (*)(void *, int_t)) zz_seti,
  (void (*)(void *, zz_t)) zz_set,
  (void (*)(void *, void *)) zz_set,
  (void (*)(void *, void *)) zz_neg,
  (void (*)(void *, void *, int_t)) zz_addi,
  (void (*)(void *, void *, zz_t)) zz_add,
  (void (*)(void *, void *, void *)) zz_add,
  (void (*)(void *, void *, void *)) zz_sub,
  (void (*)(void *, void *, int_t)) zz_muli,
  (void (*)(void *, void *, zz_t)) zz_mul,
  (void (*)(void *, void *, void *)) zz_mul,
  (char * (*)(void *)) zz_getstr,
  NULL, NULL
};

