#include "scheduler.h"

#include <pthread.h>

#define QUEUE_CAPACITY 32000

Request queue[QUEUE_CAPACITY];
int head = 0;
int tail = 0;
int count = 0;

pthread_mutex_t lock;


void schedulerInit() {
    pthread_mutex_init(&lock, NULL);
    head = 0;
    tail = 0;
    count = 0;
}

void schedulerPush(Request request) {
    while (1) {
        pthread_mutex_lock(&lock);

        if (count < QUEUE_CAPACITY) {
            queue[tail] = request;
            tail = (tail + 1 ) % QUEUE_CAPACITY;
            count++;
            pthread_mutex_unlock(&lock);
            return;
        }

        pthread_mutex_unlock(&lock);
    }
}

int schedulerPop(Request *out) {
    while (1) {
        pthread_mutex_lock(&lock);

        if (count != 0) {
            out[0] = queue[head];
            head = (head + 1) % QUEUE_CAPACITY;
            count --;
            pthread_mutex_unlock(&lock);
            return 1;
        }

        pthread_mutex_unlock(&lock);
    }
}