#include "cache.h"
#include <string.h>
#include <pthread.h>

#define CACHE_SIZE 4096

typedef struct {
    uint8_t hash[32];
    uint64_t answer;
    int used;
} cacheEntry;

static pthread_mutex_t cacheLock = PTHREAD_MUTEX_INITIALIZER;
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

int cacheLookup(uint8_t *hash, uint64_t *answer) {
    pthread_mutex_lock(&cacheLock);
    int response = findSlot(hash);
    if (response == -1) {
        pthread_mutex_unlock(&cacheLock);
        return 0;
    }
    else if (cache[response].used == 1) {
        *answer = cache[response].answer;
        pthread_mutex_unlock(&cacheLock);
        return 1;
    }
    else {
        pthread_mutex_unlock(&cacheLock);
        return 0;
    }
}

void cacheInsert(uint8_t *hash, uint64_t answer) {
    pthread_mutex_lock(&cacheLock);
    int response = findSlot(hash);
    if (response == -1) {
        pthread_mutex_unlock(&cacheLock);
        return;
    }
    memcpy(cache[response].hash, hash, 32);
    cache[response].answer = answer;
    cache[response].used = 1;
    pthread_mutex_unlock(&cacheLock);
}