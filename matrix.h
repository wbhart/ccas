#include <stdio.h>
#include "parent.h"

typedef struct
{
    void * entries;
    int_t r;
    int_t c;
    void ** rows;
    parent_struct * parent;
} Mat_struct;

typedef Mat_struct Mat_t[1];


void mat_init(Mat_t mat, parent_t parent, int_t rows, int_t cols);

void mat_clear(Mat_t mat);

void mat_fit(Mat_t mat, int_t len);

void mat_set(Mat_t a, const Mat_t b);
