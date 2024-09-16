#ifndef STDDEF_H
#define STDDEF_H

#ifndef TARGET_N64
#include <libultra/types.h>
#else
#include <PR/ultratypes.h>
#endif

#ifndef offsetof
#define offsetof(st, m) ((size_t) & (((st*) 0)->m))
#endif

#endif
