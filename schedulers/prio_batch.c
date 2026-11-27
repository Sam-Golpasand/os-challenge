#include "scheduler.h"
#include <pthread.h>

#define QUEUE_CAPACITY 16384

typedef struct {
    Request queue[QUEUE_CAPACITY];
    int head;
    int tail;
    int count;
} Queue;

// Here we have index 0 to be a priority of 1 and 15 to be 16.
Queue priorityQueue[16];
int totalCount;

pthread_mutex_t lock;
pthread_cond_t notFull;
pthread_cond_t notEmpty;


void schedulerInit() {
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&notFull, NULL);
    pthread_cond_init(&notEmpty, NULL);
    for (int i = 0; i < 16; i++) {
        priorityQueue[i].count = 0;
        priorityQueue[i].head = 0;
        priorityQueue[i].tail = 0;
    }
}

void schedulerPush(Request request) {
    int pIndex = request.priority - 1;

    pthread_mutex_lock(&lock);

    while (priorityQueue[pIndex].count == QUEUE_CAPACITY) {
        pthread_cond_wait(&notFull, &lock);
    }

    priorityQueue[pIndex].queue[priorityQueue[pIndex].tail] = request;
    priorityQueue[pIndex].tail = (priorityQueue[pIndex].tail + 1 ) % QUEUE_CAPACITY;
    priorityQueue[pIndex].count++;
    totalCount++;

    pthread_cond_signal(&notEmpty);

    pthread_mutex_unlock(&lock);
}

int schedulerPop(Request *out, int batchSize) {
    pthread_mutex_lock(&lock);

    while (totalCount == 0) {
        pthread_cond_wait(&notEmpty, &lock);
    }

    int popped = 0;

    for (int i = 15; i >= 0 && popped < batchSize; i--) {
        while (priorityQueue[i].count > 0 && popped < batchSize) {
            out[popped] = priorityQueue[i].queue[priorityQueue[i].head];
            priorityQueue[i].head = (priorityQueue[i].head + 1) % QUEUE_CAPACITY;
            priorityQueue[i].count--;
            totalCount--;
            popped++;
        }
    }

    pthread_cond_signal(&notFull);

    pthread_mutex_unlock(&lock);
    return popped;
}