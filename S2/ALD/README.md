# Algorithmen und Datenstrukturen

### Basics
- 1💠[What is an algorithm?](https://www.tutorialspoint.com/data_structures_algorithms/algorithms_basics.htm#)
- 2💠[Algorithm performance, aka Big O notation and Landau symbols](https://web.mit.edu/16.070/www/lecture/big_o.pdf)

<details>
  <summary>1💠How to keep track of time in C?</summary>
  
```c
#include <time.h>
clock_t t1 = clock();
// do time intesive task here...
clock_t t2 = clock();
double duration = (double)(t2 - t1)/(double)CLOCKS_PER_SEC;
```
</details>

<details>
  <summary>1💠How to generate random numbers in C?</summary>
  
```c
int number = rand() << 16+rand();
// rand() only returns 16 bit numbers max,
// thus we do the trick above to get max 32bit numbers
```
</details>
  
<details>
  <summary>1💠How to compare strings in C?</summary>
  
```c
char str1[] = "abcd", str2[] = "abCd", str3[] = "abcd";
int result = strcmp(str1, str2); // strcmp aka stringcompare returns 0 if strings are equal
```
</details>

### Algorithms
- [1💠Selection sort](https://www.geeksforgeeks.org/selection-sort/)
  
### Data structures
- [1💠Doubly Linked List](https://www.geeksforgeeks.org/doubly-linked-list/?ref=lbp)
- [1💠Binary Search Tree](https://www.geeksforgeeks.org/binary-search-tree-data-structure/?ref=gcse)
