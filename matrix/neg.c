#include "ccas.h"
#include "matrix.h"

void mat_neg(Mat_t a, Mat_t b)
{
    void (* neg)(void *, void *) = a->parent->base->neg;
    int_t size = a->parent->base->size;

    int_t i;
    int_t j;

    for(i = 0; i < b->r; i++)
        for(j = 0; j < c; j++)
            neg(a->rows[i] + (j), b->rows[i] + (j));
}
