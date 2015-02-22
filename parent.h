#include <stdio.h>
#include "zz.h"

#ifndef PARENT_H
#define PARENT_H

/*****************************************************************************

   Parent struct

*****************************************************************************/

typedef enum
{
   INT_T, POLY_T, FRAC_T, RES_T, MAT_T
} tag_t;

typedef struct parent_struct
{
   tag_t tag;
   int_t size;
   void (* init)(void *, struct parent_struct *);
   void (* clear)(void *);
   void (* zero)(void *);
   int (* iszero)(void *);
   void (* seti)(void *, int_t);
   void (* setz)(void *, zz_t);
   void (* set)(void *, void *);
   void (* neg)(void *, void *);
   void (* addi)(void *, void *, int_t);
   void (* addz)(void *, void *, zz_t);
   void (* add)(void *, void *, void *);
   void (* sub)(void *, void *, void *);
   void (* muli)(void *, void *, int_t);
   void (* mulz)(void *, void *, zz_t);
   void (* mul)(void *, void *, void *);
   char * (*getstr)(void *);
   struct parent_struct * base;
   void * aux;
} parent_struct;

typedef parent_struct parent_t[1];

/*****************************************************************************

   Integer ring

*****************************************************************************/

extern parent_t ZZ;

#endif

