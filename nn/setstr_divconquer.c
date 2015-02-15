#include <math.h>
#include "ccas.h"
#include "nn.h"

int_t nn_setstr_divconquer(nn_t a, const char * str, int_t digits,
                                                nn_t * tree, int_t * tn)
{
   if (digits < SETSTR_DIVCONQUER_THRESHOLD)
      return nn_setstr_classical(a, str, digits);

   TMP_INIT;

   int_t depth = 1;
   while ((1L << (depth + 1)) < digits)
      depth++;

   int_t lo = (1L << depth);
   int_t hi = digits - lo;
   uint_t cy;

   /* 0.103810253 is log(10/log(2^32) */
   int_t lwords = (int_t) ceil(lo*0.103810253*(INT_BITS/32));
   int_t hwords = (int_t) ceil(hi*0.103810253*(INT_BITS/32));
   
   TMP_START;
   nn_t t1 = (nn_t) TMP_ALLOC(lwords*sizeof(uint_t));
   nn_t t2 = (nn_t) TMP_ALLOC(hwords*sizeof(uint_t));
   nn_t p = (nn_t) TMP_ALLOC((lwords + tn[depth])*sizeof(uint_t));

   lwords = nn_setstr_divconquer(t1, str + hi, lo, tree, tn);
   hwords = nn_setstr_divconquer(t2, str, hi, tree, tn);

   nn_mul(p, tree[depth], tn[depth], t2, hwords);
   hwords = nn_normalise(p, hwords + tn[depth]);

   if (hwords < lwords)
      cy = nn_add(a, t1, lwords, p, hwords);
   else
      cy = nn_add(a, p, hwords, t1, lwords);
      
   lwords += hwords;
   if (cy != 0)
      a[lwords++] = cy;      

   TMP_END;

   return lwords;
}
