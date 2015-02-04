#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <alloca.h>
#include "config.h"

#define int_t int64_t
#define uint_t uint64_t
#define wide_t __int128
#define INT_BITS 64
#define INT_HIBIT (1UL << 63)
#define RANDSEED1 13845646450878251009UL
#define RANDSEED2 13142370077570254774UL 
#define RANDMUL1 13282407956253574709UL
#define RANDMUL2 7557322358563246341UL
#define RANDADD 286824421UL
#define count_leading_zeros __builtin_clzl

#if WANT_ASSERT
#define CCAS_ASSERT assert
#else
#define CCAS_ASSERT(x)
#endif

#define CCAS_MIN(x, y) \
   ((x) < (y) ? (x) : (y))

#define CCAS_MAX(x, y) \
   ((x) >= (y) ? (x) : (y))

#define CCAS_ABS(x) \
   ((x) >= 0 ? (x) : -(x))

#define NORMALISED(x) \
   (((x) & INT_HIBIT) != 0)

#define BITCOUNT(x) \
   ((x) == 0 ? 0 : INT_BITS - count_leading_zeros(x))

#define POWEROF2(x) \
   ((x) != 0 && ((x) & ((x) - 1)) == 0)

#define TMP_INIT                  \
   typedef struct __tmp_struct {  \
      void * block;               \
      struct __tmp_struct * next; \
   } __tmp_t;                     \
   __tmp_t * __tmp_root;          \
   __tmp_t * __t

#define TMP_START \
   __tmp_root = NULL

#define TMP_ALLOC(size)                                  \
   ((size) > 8192 ?                                      \
      (nn_t) (__t = (__tmp_t *) alloca(sizeof(__tmp_t)), \
       __t->next = __tmp_root,                           \
       __tmp_root = __t,                                 \
       __t->block = (nn_t) malloc(size)) :               \
      (nn_t) alloca(size))

#define TMP_END                      \
   while (__tmp_root) {              \
      free(__tmp_root->block);       \
      __tmp_root = __tmp_root->next; \
   }

