#define LONESHA256_STATIC
#include "lonesha256.h"
#if defined(__linux__)
  #include <endian.h>
#elif defined(__APPLE__)
  #include <libkern/OSByteOrder.h>
  #define htobe64(x) OSSwapHostToBigInt64(x)
  #define htole64(x) OSSwapHostToLittleInt64(x)
  #define be64toh(x) OSSwapBigToHostInt64(x)
  #define le64toh(x) OSSwapLittleToHostInt64(x)
#endif
#include <stdint.h>
#include <string.h>



int decrypt(uint8_t *buffer, uint64_t *answer) {

    uint8_t out[32] = {0};

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

            *answer = htobe64(i);
            
            return 0;
            
        }
    }
    return 1;
    
}