#ifndef OS_CHALLENGE_CACHE_H
#define OS_CHALLENGE_CACHE_H

#include <stdint.h>

int cacheLookup(uint8_t *hash, uint64_t *answer);
void cacheInsert(uint8_t *hash, uint64_t answer);

#endif
