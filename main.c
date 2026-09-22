#include "./utils/tcpHandler.h"
#include <stdlib.h>

int main(int argc, char **argv) {

    if (argc > 1) { 
        startServer(atoi(argv[1]));
    }
    
    return 0;
}