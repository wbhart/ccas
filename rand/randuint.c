#include "ccas.h"
#include "rand.h"

uint_t randuint(rand_t state, uint_t limit) 
{
   uint_t rnd = randword(state);

   return limit == 0 ? rnd : rnd % limit;
}
