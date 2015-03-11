#include "ccas.h"
#include "matrix.h"

void mat_clear(Mat_t mat)
{
    void (* clear)(void *) = mat->parent->base->clear;
    int_t size = mat->parent->base->size;

    int_t i;

    for(i = 0; i < (mat->r) * (mat->c); i++)
        clear(mat->entries + i);

    free(mat->entries);
    free(mat->rows);
}
