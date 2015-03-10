#include "ccas.h"
#include "matrix.h"

void mat_sub(Mat_t res, Mat_t a, Mat_t b)
{
    const int_t rows = a->r;
    const int_t cols = a->c;
    int_t i;
    int_t j;

    void (*sub)(void *, void *, void *) = res->parent->base->sub;
    void (*set)(void *, void *) = res->parent->base->set;
    void (*neg)(void *, void *) = a->parent->base->neg;

    for(i = 0; i < rows; i++)
        for(j = 0; j < cols; j++)
            sub(res->rows[i] + (j), a->rows[i] + (j), b->rows[i] + (j));
}
