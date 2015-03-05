#include <stdio.h>
#include "parent.h"

typedef struct
{
    void * elements;
    int_t rows;
    int_t cols;
    int_t alloc;
    parent_struct * parent;
} Mat_struct;

typedef Mat_struct Mat_t[1];



void mat_init(Mat_t mat, parent_t parent);

void mat_clear(Mat_t mat);

void mat_fit(Mat_t mat, int_t len);

void mat_set(Mat_t a, Mat_t b);
