#include "ccas.h"
#include "matrix.h"

void mat_fit(Mat_t mat, int_t len)
{
    if(len > mat->alloc)
    {
        void (* init)(void *, parent_t) = mat->parent->base->init;
        int_t size = mat->parent->base->size;

        len = CCAS_MAX(len, 2*mat->alloc);

        mat->elements = realloc(mat->elements, size*len);

        for(int_t i = mat->alloc; i < len; i++)
            init(mat->elements + i*size, mat->parent->base);

        mat->alloc = len;
    }
}

