#include "ccas.h"
#include "matrix.h"

void mat_set(Mat_t a, Mat_t b)
{
    if(a != b)
    {
        void (* set)(void *, void *) = a->parent->base->set;
        int_t size = a->parent->base->size;

        int_t length = (b->rows)*(b->cols);
        mat_fit(a, length);

        for (int_t i = 0; i < length; i++)
            set(a->elements + i*size, b->elements + i*size);

        a->rows = b->rows;
        a->cols = b->cols;
    }
}

