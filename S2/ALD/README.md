# Algorithmen und Datenstrukturen
(Algorithms and Datastructures)

### Basics
- 1๐ [What is an algorithm?](https://www.tutorialspoint.com/data_structures_algorithms/algorithms_basics.htm#)
- 2๐ Understanding [algorithm performance](https://en.wikipedia.org/wiki/Algorithmic_efficiency), [time complexity](https://towardsdatascience.com/understanding-time-complexity-with-python-examples-2bda6e8158a7), [Big O notation and Landau symbols.](https://web.mit.edu/16.070/www/lecture/big_o.pdf)
- 2๐ [What is a mathematic proof?](https://en.wikipedia.org/wiki/Mathematical_proof#Direct_proof)
- 2๐ [How to prove a functions/algorithms time complexity?](https://www.google.com/search?q=How+to+prove+a+functions%2Falgorithms+time+complexity)
- 0.5๐ [What are left/right shift operators in C?](https://www.geeksforgeeks.org/left-shift-right-shift-operators-c-cpp/)
- 3๐ [What is an adjacency Matrix](https://en.wikipedia.org/wiki/Adjacency_matrix) and an [adjacency list?](https://en.wikipedia.org/wiki/Adjacency_list)
- 3๐ [What is meant by lexicographix order?](https://en.wikipedia.org/wiki/Lexicographic_order)
- 3๐ [The Single-Source-Shortest-Path-Problem.](https://www.google.com/search?q=Single-Source-Shortest-Path-Problem)
- 1๐ [How does a distance matrix work?](https://en.wikipedia.org/wiki/Distance_matrix)

<details>
  <summary>1๐ How to keep track of time in C?</summary>
  
```c
#include <time.h>
clock_t t1 = clock();
// do time intesive task here...
clock_t t2 = clock();
double duration = (double)(t2 - t1)/(double)CLOCKS_PER_SEC;
```
</details>

<details>
  <summary>1๐ How to generate random numbers in C?</summary>
  
```c
int number = rand() << 16+rand();
// rand() only returns 16 bit numbers max,
// thus we do the trick above to get max 32bit numbers
```
</details>
  
<details>
  <summary>1๐ How to compare strings in C?</summary>
  
```c
char str1[] = "abcd", str2[] = "abCd", str3[] = "abcd";
int result = strcmp(str1, str2); // strcmp aka stringcompare returns 0 if strings are equal
```
</details>

### Algorithms
- 1๐ [Selection Sort](https://www.geeksforgeeks.org/selection-sort/)
- 1๐ [Quick Sort](https://www.geeksforgeeks.org/quick-sort/)
- 1๐ [Merge Sort](https://www.geeksforgeeks.org/merge-sort/)
- 0.5๐ [Heap Sort](https://www.geeksforgeeks.org/heap-sort)
- 0.5๐ [Radix Sort](https://www.geeksforgeeks.org/radix-sort/)
- 0.5๐ [Binary Search](https://www.geeksforgeeks.org/binary-search/)
- 0.5๐ [Interpolation Search](https://www.geeksforgeeks.org/interpolation-search/)
- 0๐ [Interpolation Search vs Binary Search](https://www.geeksforgeeks.org/g-fact-84)
- 2๐ [Hashing](https://www.geeksforgeeks.org/hashing-set-1-introduction/)
- 3๐ [Prims Algorithm](https://www.geeksforgeeks.org/prims-minimum-spanning-tree-mst-greedy-algo-5/)
- 3๐ [Dijkstras Shortest Path Algorithm](https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/)
- 3๐ [Floyd-Warshall Algorithm](https://www.geeksforgeeks.org/floyd-warshall-algorithm-dp-16/)
- 1๐ [Levenshtein-Distance](https://www.geeksforgeeks.org/java-program-to-implement-levenshtein-distance-computing-algorithm/) and [Edit-Distance](https://en.wikipedia.org/wiki/Edit_distance)
  
### Data structures
- 1๐ [Doubly Linked List](https://www.geeksforgeeks.org/doubly-linked-list)
- 1๐ [Binary Search Tree](https://www.geeksforgeeks.org/binary-search-tree-data-structure)
- 3๐ [Minimum Spanning Tree](https://www.geeksforgeeks.org/properties-of-minimum-spanning-tree-mst/)
