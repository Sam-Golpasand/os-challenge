#include "tcpHandler.h"
#include "lonesha256.h"
#define LONESHA256_IMPLEMENTATION
#include <asm-generic/socket.h>
#include <endian.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

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
    uint8_t buffer[49] = {0};
    uint8_t out[32] = {0};

    // create a socket
    int connectionRes = serverFD = socket(AF_INET, SOCK_STREAM, 0);

    if (connectionRes < 0) {
       printf("Server couldn't create a socket");
      exit(EXIT_FAILURE); 
    }

    int opt = 1;
    // change the options for the socket
    int socketSettingsRes = setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (socketSettingsRes < 0) {
        perror("setsocketopt failed");
        exit(EXIT_FAILURE);
    }

    // This tell us to use the TCP (AD_INET) and accept any ip address like the specs say. 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Here we bind the socket using the serverFD on to the address. The size of the address is also given
    int bindRes = bind(serverFD, (struct sockaddr *)&address, sizeof(address));

    if (bindRes < 0) {
        printf("bind failed");
        exit(EXIT_FAILURE);
    }

    // Here we actually start to listen using the serverFD setting we have made. 
    // Note that the integer we give is the queue length accepted. If it surpasses 10 we deny them
    int listenRes = listen(serverFD, 10);

    if (listenRes < 0) {
        printf("Failed to listen");
        exit(EXIT_FAILURE);
    }

    while(1) {
        printf("Waiting for connection...\n");

        if ((newSocket = accept(serverFD, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept failed");
            continue;
        }

        printf("Client connected: %s\n", inet_ntoa(address.sin_addr));

        ssize_t valRead;
        valRead = read(newSocket, buffer, 49);
        if (valRead < 0) {
            perror("read failed");
            close(newSocket);
            continue;
        }

        // we start by splitting up the raw data based on the spec
        uint64_t start = *(uint64_t *)(&buffer[32]);
        uint64_t end = *(uint64_t *)(&buffer[40]);
        

        // then we convert it from big endian to host
        start = be64toh(start);
        end = be64toh(end);
        

        for (uint64_t i = start; i < end; i++) {
            // since lonesha takes in little endian we convert the input and hash
            uint64_t input = htole64(i); 
            lonesha256(out, (uint8_t *)&input, sizeof(input));
    
            // check it the hash is correct, else continue
            if (memcmp(buffer, out, 32) == 0) {
                uint64_t answer = htobe64(i);
                send(newSocket, &answer, sizeof(answer), 0);
                
                break;
            }
        }
        close(newSocket);
    }

    return 0;
}