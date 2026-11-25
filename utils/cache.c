#include "cache.h"
#include <string.h>

#define CACHE_SIZE 4096

typedef struct {
    uint8_t hash[32];
    uint64_t answer;
    int used;
} cacheEntry;

static cacheEntry cache[CACHE_SIZE];

static int findSlot(uint8_t hash[]) {
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (memcmp(cache[i].hash, hash, 32) == 0) {
            return i;
        }
        if (cache[i].used == 0) {
            return i;
        }
    }
    return -1;
}