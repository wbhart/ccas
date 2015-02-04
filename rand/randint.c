#include "ccas.h"
#include "rand.h"

int_t randint(rand_t state, int_t limit) 
{
   CCAS_ASSERT(limit != 0);

   uint_t rnd = randword(state);

   if (limit >= 0)
       return (int_t) (limit == 0 ? rnd : rnd % limit);
   else
   {
       rnd %= -limit;
       
       return (int_t) (randword(state) & 1 ? rnd : -rnd);
   }
}
