#include "ccas.h"
#include "rand.h"

uint_t randbits(rand_t state, int_t bits)
{
   CCAS_ASSERT(bits >= 0);
   CCAS_ASSERT(bits <= INT_BITS);

   /* Ensure msb is set */
   const uint_t msb = bits == 0 ? 0 : 1UL << (bits - 1);
   const uint_t limit = bits == INT_BITS ? 0 : 1UL << bits;
   
   return msb | randuint(state, limit);
}
