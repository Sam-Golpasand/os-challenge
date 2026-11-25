#include "cache.h"
#include <string.h>

#define CACHE_SIZE 4096

typedef struct {
    uint8_t hash;
    uint64_t answer;
    int used;
} cacheEntry;

static cacheEntry cache[CACHE_SIZE];

