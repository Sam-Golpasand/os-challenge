The project structure is basically a main file in the root that is our main access point and a utils folder. The utils folder will hold the functions needed, so the `tcpHandler.c` which is in charge of opening the socket and the `decrypt.c` that actually cracks the hash. 

In regard to the `MakeFile` we used wildcards to make development easier where we just include everything inside the utils folder. We might change this in the future if any of the experiments are too complex and will convolute the single utils folder. But so far it works

We also wanted to preface the `tcpHandler.c` with the fact that a lot of the boiler plate code has gotten inspiration from a youtube tutorial found here, with the required changes added by us. The group does though understand the tcp process and what layers we go through, from what code.

We did include the `lonesha256.h` in the utils file. This was done for local development reasons. I personally use a linux machine so i didn't try to set up the vagrant VM until i wanted to test it. When i wanted to do so i came accross so many difficulties so it has actually not been tested in that enviourment. We will setup the test enviourment for the actual final submission. 

If any libraries we aren't allowed to use is used, please let us know!