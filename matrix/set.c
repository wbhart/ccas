#include "ccas.h"
#include "matrix.h"

void mat_set(Mat_t a, const Mat_t b)
{
    if(a != b)
    {
        void (* set)(void *, void *) = a->parent->base->set;
        int_t size = a->parent->base->size;

        int_t i;
        int_t j;
        int_t rows = b->r;
        int_t cols = b->c;

        if((b->r)!=0 && (b->c)!=0)
            for (i = 0; i < rows; i++)
                for(j = 0; j < cols; j++)
                    set(a->rows[i] + (j), b->rows[i] + (j));

    }
}
