#include "tcpHandler.h"
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "decrypt.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "cache.h"
#include "schedulers/scheduler.h"

#define CORE_COUNT 8

void *workerThread(void *arg) {
    (void)arg;

    Request batch[MAX_BATCH_SIZE];

    while (1) {
        int n = schedulerPop(batch, MAX_BATCH_SIZE);

        for (int i = 0; i < n; i++) {
            uint64_t answer = 0;

            // check the cache first, only crack the hash on a miss
            int response = cacheLookup(batch[i].buffer, &answer);
            if (response == 0) {
                decrypt(batch[i].buffer, &answer);
                cacheInsert(batch[i].buffer, answer);
            }

            send(batch[i].client, &answer, sizeof(answer), 0);
            close(batch[i].client);
        }
    }
}


int startServer(int port) {

    // The serverFD is used for the initialization of the port. It is basically our setting for
    // what port to use and listen on, and basically just waits for new connections.
    // An important thing about serverFD is that it acts as an API between our program and the OS
    // so we actually just use it like a pointer that tells the os what to do
    // The NewSocket is the one we use for comunnication. When the serverFD sees a new connection
    // incoming, we use an accept() function which creates a newSocket which will overwrite the one below
    int serverFD, newSocket;

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // create a socket and check if it is created properly
    if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
       printf("Server couldn't create a socket");
      exit(EXIT_FAILURE);
    }

    int opt = 1;
    // change the options for the socket
    // The reason for the opt variable is that the function expects an address.
    if ((setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) < 0) {
        perror("setsocketopt failed");
        exit(EXIT_FAILURE);
    }

    // This tell us to use the TCP (AD_INET) and accept any ip address like the specs say.
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Here we bind the socket using the serverFD on to the address. The size of the address is also given
    if (bind(serverFD, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("bind failed");
        exit(EXIT_FAILURE);
    }

    // Here we actually start to listen using the serverFD setting we have made.
    // Note that the integer we give is the queue length accepted. If it surpasses 10 we deny them

    if (listen(serverFD, 10000) < 0) {
        printf("Failed to listen");
        exit(EXIT_FAILURE);
    }

    schedulerInit();

    pthread_t workers[CORE_COUNT];

    for (int i = 0; i < CORE_COUNT; i++) {
        if (pthread_create(&workers[i], NULL, workerThread, NULL) != 0) {
            perror("Failed to create threads");
            exit(EXIT_FAILURE);
        }
    }

    while(1) {

        if ((newSocket = accept(serverFD, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept failed");
            continue;
        }

        uint8_t buffer[49] = {0};

        // Since read() function doesnt gaurentee all 49 bytes gets read, we have to add that
        // safety ourselves. 
        size_t total = 0;
        while (total < sizeof(buffer)) {
            ssize_t n = read(newSocket, buffer + total, sizeof(buffer) - total);
            if (n < 0 && errno == EINTR) {
                continue;
            }
            if (n <= 0) {
                break;
            }
            total += n;
        }
        if (total < sizeof(buffer)) {
            close(newSocket);
            continue;
        }

        Request task;
        task.client = newSocket;
        memcpy(task.buffer, buffer, 49);
        task.priority = buffer[48];

        schedulerPush(task);
    }

    return 0;
}
