#include "ccas.h"
#include "Poly.h"

void poly_init(Poly_t pol, parent_t parent)
{
   pol->coeffs = NULL;
   pol->length = 0;
   pol->alloc = 0;
   pol->parent = parent;
}

