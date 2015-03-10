#include "ccas.h"
#include "matrix.h"

void mat_add(Mat_t res, const Mat_t mat1, const Mat_t mat2)
{
    if (res->c < 1)
        return;

    int_t i;
    int_t j;
    void (* add)(void *, void *, void *) = res->parent->base->add;

    for (i = 0; i < res->r; i++)
        for (j = 0; j < res->c; j++)
            add(res->rows[i] + (j), mat1->rows[i] + (j), mat2->rows[i] + (j));
}
