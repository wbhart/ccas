#include "ccas.h"
#include "matrix.h"

void mat_clear(Mat_t mat)
{
    void (* clear)(void *) = mat->parent->base->clear;
    int_t size = mat->parent->base->size;

    //for(int_t i = 0; i < mat->alloc; i++)
      //  clear(mat->
      
}

