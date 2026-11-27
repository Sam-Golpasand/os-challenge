#include "scheduler.h"

#include <pthread.h>

#define QUEUE_CAPACITY 32000

Request queue[QUEUE_CAPACITY];
int head = 0;
int tail = 0;
int count = 0;

pthread_mutex_t lock;
pthread_cond_t notFull;
pthread_cond_t notEmpty;


void schedulerInit() {
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&notFull, NULL);
    pthread_cond_init(&notEmpty, NULL);
    head = 0;
    tail = 0;
    count = 0;
}

void schedulerPush(Request request) {
    pthread_mutex_lock(&lock);

    while (count == QUEUE_CAPACITY) {
        pthread_cond_wait(&notFull, &lock);
    }

    queue[tail] = request;
    tail = (tail + 1 ) % QUEUE_CAPACITY;
    count++;

    pthread_cond_signal(&notEmpty);

    pthread_mutex_unlock(&lock);
}

int schedulerPop(Request *out, int batchSize) {
    pthread_mutex_lock(&lock);

    while (count == 0) {
        pthread_cond_wait(&notEmpty, &lock);
    }

    out[0] = queue[head];
    head = (head + 1) % QUEUE_CAPACITY;
    count --;

    pthread_cond_signal(&notFull);

    pthread_mutex_unlock(&lock);
    return 1;
}