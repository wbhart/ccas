#include "ccas.h"
#include "matrix.h"

void mat_init(Mat_t mat, parent_t parent, int_t rows, int_t cols)
{
    if ((rows!=0) && (cols!=0))       /* Allocate space for r*c small entries */
    {
        int_t i;
        mat->entries = (void *) malloc(rows * cols * mat->parent->base->size);
        mat->rows = (void **) malloc(rows * sizeof(mat->parent->base));    /* Initialise rows */

        for (i = 0; i < rows; i++)
            mat->rows[i] = mat->entries + i * cols;
    }
    else
        mat->entries = NULL;

    mat->r = rows;
    mat->c = cols;
    mat->parent = parent;
}
