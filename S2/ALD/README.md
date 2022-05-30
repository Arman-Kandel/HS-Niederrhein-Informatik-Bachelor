# Algorithmen und Datenstrukturen
(Algorithms and Datastructures)

### Basics
- 1💠[What is an algorithm?](https://www.tutorialspoint.com/data_structures_algorithms/algorithms_basics.htm#)
- 2💠Understanding [algorithm performance](https://en.wikipedia.org/wiki/Algorithmic_efficiency), [time complexity](https://towardsdatascience.com/understanding-time-complexity-with-python-examples-2bda6e8158a7), [Big O notation and Landau symbols.](https://web.mit.edu/16.070/www/lecture/big_o.pdf)
- 2💠[What is a mathematic proof?](https://en.wikipedia.org/wiki/Mathematical_proof#Direct_proof)
- 2💠[How to prove a functions/algorithms time complexity?](https://www.google.com/search?q=How+to+prove+a+functions%2Falgorithms+time+complexity)
- 0.5💠[What are left/right shift operators in C?](https://www.geeksforgeeks.org/left-shift-right-shift-operators-c-cpp/)
- 1💠[What is an adjacency Matrix](https://en.wikipedia.org/wiki/Adjacency_matrix) and an [adjacency list?](https://en.wikipedia.org/wiki/Adjacency_list)
- 1💠[What is meant by lexicographix order?](https://en.wikipedia.org/wiki/Lexicographic_order)
- 1💠[The Single-Source-Shortest-Path-Problem.](https://www.google.com/search?q=Single-Source-Shortest-Path-Problem)

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
- 1💠[Selection Sort](https://www.geeksforgeeks.org/selection-sort/)
- 1💠[Quick Sort](https://www.geeksforgeeks.org/quick-sort/)
- 1💠[Merge Sort](https://www.geeksforgeeks.org/merge-sort/)
- 0.5💠[Heap Sort](https://www.geeksforgeeks.org/heap-sort)
- 0.5💠[Radix Sort](https://www.geeksforgeeks.org/radix-sort/)
- 0.5💠[Binary Search](https://www.geeksforgeeks.org/binary-search/)
- 0.5💠[Interpolation Search](https://www.geeksforgeeks.org/interpolation-search/)
- 0💠[Interpolation Search vs Binary Search](https://www.geeksforgeeks.org/g-fact-84)
- 2💠[Hashing](https://www.geeksforgeeks.org/hashing-set-1-introduction/)
- 1💠[Prims greedy algorithm](https://www.geeksforgeeks.org/prims-minimum-spanning-tree-mst-greedy-algo-5/)
  
### Data structures
- 1💠[Doubly Linked List](https://www.geeksforgeeks.org/doubly-linked-list)
- 1💠[Binary Search Tree](https://www.geeksforgeeks.org/binary-search-tree-data-structure)
- 1💠[Minimum Spanning Tree](https://www.geeksforgeeks.org/properties-of-minimum-spanning-tree-mst/)
