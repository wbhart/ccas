#include "ccas.h"
#include "rand.h"

uint_t randword(rand_t state) 
{   
    state->rand1 = (state->rand1 * RANDMUL1 + RANDADD);
    state->rand2 = (state->rand2 * RANDMUL2 + RANDADD);
    const uint_t bits = INT_BITS/2;

    return (state->rand1 >> bits) + ((state->rand2 >> bits) << bits);
}

