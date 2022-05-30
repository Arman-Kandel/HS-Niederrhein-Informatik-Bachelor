# Algorithmen und Datenstrukturen

### Basics
- [What is an algorithm?](https://www.tutorialspoint.com/data_structures_algorithms/algorithms_basics.htm#)
<details>
  <summary>How to keep track of time?</summary>
  
```c
#include <time.h>
clock_t t1 = clock();
// do time intesive task here...
clock_t t2 = clock();
double duration = (double)(t2 - t1)/(double)CLOCKS_PER_SEC;
```
</details>

