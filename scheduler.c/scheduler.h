#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

typedef struct {
    int client;
    uint8_t buffer[49];
    uint8_t priority;
} Request;

void schedulerInit();

void schedulerPush(Request request);

int schedulerPop(Request *request);


#endif