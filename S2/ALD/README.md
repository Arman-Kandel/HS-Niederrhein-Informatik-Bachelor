# Algorithmen und Datenstrukturen

### Basics
- [What is an algorithm?](https://www.tutorialspoint.com/data_structures_algorithms/algorithms_basics.htm#)
- [Selection sort](https://www.geeksforgeeks.org/selection-sort/)
- [

<details>
  <summary>How to keep track of time in C?</summary>
  
```c
#include <time.h>
clock_t t1 = clock();
// do time intesive task here...
clock_t t2 = clock();
double duration = (double)(t2 - t1)/(double)CLOCKS_PER_SEC;
```
</details>

<details>
  <summary>How to generate random numbers in C?</summary>
  
```c
int number = rand() << 16+rand();
// rand() only returns 16 bit numbers max,
// thus we do the trick above to get max 32bit numbers
```
</details>

### Algorithms
  
### Data structures
- [Doubly Linked List](https://www.geeksforgeeks.org/doubly-linked-list/?ref=lbp)
