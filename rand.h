#include "ccas.h"

#ifndef RAND_H
#define RAND_H

typedef struct
{
    uint_t rand1;
    uint_t rand2;
} rand_s;

typedef rand_s rand_t[1];

/**
   Initialise a random state for use. The struct is seeded with values
   defined in ccas.h.
*/
static inline
void randinit(rand_t state)
{
    state->rand1 = RANDSEED1;
    state->rand2 = RANDSEED2;
}

/**
   Return a uniformly random uint_t.
*/
uint_t randword(rand_t state);

/**
   Return a uniformly random uint_t in the range [0, limit). If limit = 0
   a random word as output by randword is returned.
*/
uint_t randuint(rand_t state, uint_t limit);

/**
   Return a uniformly random int_t in the range [0, limit) if limit > 0
   otherwise in the range (limit, -limit) if limit < 0.
   If limit = 0 a random value across the full range of possible int_t's
   is returned.
*/
int_t randint(rand_t state, int_t limit);

/**
   Return a uniformly random uint_t in the range [0, 2^bits) bit with
   the high bit set so the number has exactly the given number of bits.
   Requires 0 <= bits <= INT_BITS.
*/
uint_t randbits(rand_t state, int_t bits);

#endif

