#include <libultraship.h>
#include <macros.h>
#include <segments.h>

#define MEMORY_POOL_SIZE 0xAB630

/**
 * Memory pool variable prevents code segments flowing into the memory pool
 * for easier portability.
*/
u8 gMemoryPool[MEMORY_POOL_SIZE];
