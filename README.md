The project structure is basically a main file in the root, that is our main access point and a utils folder. The utils folder will hold the functions needed, so the `tcpHandler.c` which is in charge of opening the socket and the `decrypt.c` that actually cracks the hash. 

In regard to the `MakeFile`, we used wildcards to make development easier, where we just include everything inside the utils folder. We might change this in the future, for example if any of the experiments are too complex and will result in the utils folder becoming too convoluted. But so far it works well.

We also wanted to preface the `tcpHandler.c` with the fact that a lot of the boiler plate code has gotten inspiration from a youtube tutorial found [here](https://www.youtube.com/watch?v=set7R_bHG_k), with the required changes added by us. The group does, though, understand the tcp process and what tcp layers the code goes through. The `Makefile` was made by us but was copied and altered to fit this course. The original is from the course 02322 Machine oriented programming.

We did include the `lonesha256.h` in the utils file. This was done for local development reasons. I personally use a linux machine, so i didn't try to set up the vagrant VM until i wanted to test it. When i wanted to do so, I came accross so many difficulties, so it has actually not been tested in that enviourment. We will setup the test enviourment for the final submission. 

If any libraries we aren't allowed to use is used, please let us know!