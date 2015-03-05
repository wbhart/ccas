#include "ccas.h"
#include "matrix.h"

void mat_init(Mat_t mat, parent_t parent)
{
    mat->elements = NULL;
    mat->rows = 0;
    mat->cols = 0;
    mat->alloc = 0;
    mat->parent = parent;
}
