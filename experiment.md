 # Scheduling Experiment



## Preface

Currently there is a working decrypting server. The way it works is by - using main.c as its gateway - opening a server on a user specified port and awaiting 49 byte requests. These request will then be decrypted using the lonesha256 algorithm. The way it currently works is by a first in first out principle - so basically a queue of encrypted strings get lined up and decrypted. This is highly ineffective as there are multiple factors being accounted for in this assignment. The scoring is based on the following equation:


$$

\text{score} = \frac{1}{\text{total}} \sum_{i=1}^{\text{total}} l_i p_i

$$


This means that the average latency score will be timed by the priority meaning, the higher the priority, the higher percieved latency there will be. So if latency was 10ms and we had a priority of 16, the percieved latency would be 160 ms, which would be detrimental to the score.


Some other algorithms exists which we will explore further during this experiment 