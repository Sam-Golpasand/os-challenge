 #include "tcpHandler.h"
#include <asm-generic/socket.h>
#include <endian.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "decrypt.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

void *connectionThread(void *newSocket);

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
    pthread_t thread;

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

    if (listen(serverFD, 10) < 0) {
        printf("Failed to listen");
        exit(EXIT_FAILURE);
    }

    int running = 0;

    while(1) {
        printf("Waiting for connection...\n");

        if ((newSocket = accept(serverFD, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept failed");
            continue;
        }

        pthread_create(&thread, NULL, connectionThread, (void *) &newSocket);

        printf("Client connected: %s\n", inet_ntoa(address.sin_addr));

    }

    pthread_join(thread, NULL);
    return 0;
}

void *connectionThread(void *newSocket) { 
    uint8_t buffer[49] = {0};
    int *socket = (int *) newSocket;
    ssize_t valRead;
    valRead = read(*socket, buffer, 49);
    if (valRead < 0) {
        perror("read failed");
        close(*socket);
    }

    uint64_t answer = 0;
    decrypt(buffer, &answer);

    send(*socket, &answer, sizeof(answer), 0);
    close(*socket);
    return NULL;
} 