# Programmentwicklung 2
(Program development 2)

### Basics
<details>
  <summary>1💠What are format-specifiers in C?</summary>
  
Format specifiers are used to specifying the input data format which will be set for the given variable.
The format should be the same as the given variable.
There are different format specifiers for different variable types like below.

- `%c` is used to input character input  for `char` variable type
- `%d` is used to input integer input for `int` variable type
- `%e`,`%E`,`%f`,`%g`,`%G` are used to input floating number for `float` variable type
- `%o` is used to input integer for `int` variable type in octal format
- `%s` is used to input string for `char[]` variable type in string or char array format.
- `%x`,`%X` are used to input integer for `int` variable type in hexadecimal format.
</details>

<details>
  <summary>1💠How to print/scan stuff to/from the terminal in C?</summary>
  
```c
#include <stdio.h>
  
int main()
{
 int i = 0;                                       
 scanf("%d", &i);
 printf("Value of i = %d\n", i); 
}
```
</details>

<details>
  <summary>1💠How to print/scan stuff to/from the terminal in C++?</summary>
  
```c++
#include <iostream>
#include <string>
using namespace std;

int main()
{
 cout << "Print a String" << endl;
 int i;
 cout << "Please enter an integer value: ";
 cin >> i;
}
```
</details>
  
<details>
  <summary>1💠How to read/write files in C?</summary>
  
```c
#include <stdio.h>
#include <stdlib.h>

int main()
{
   // WRITE
  
   int num;
   FILE *fptr = fopen("C:\\program.txt","w"); // use appropriate location if you are using MacOS or Linux
   if(fptr == NULL){
      printf("Error!");   
      exit(1);             
   }

   printf("Enter num: ");
   scanf("%d",&num);

   fprintf(fptr,"%d",num);
   fclose(fptr);
  
   // READ

   if ((fptr = fopen("C:\\program.txt","r")) == NULL){
       printf("Error! opening file");
       exit(1);
   }

   fscanf(fptr,"%d", &num);

   printf("Value of n=%d", num);
   fclose(fptr); 

   return 0;
}
```
</details>
  
<details>
  <summary>1💠How to read/write files in C++?</summary>
  
```c
#include <iostream>
#include <fstream>
using namespace std;

int main () {
  // WRITE
  ofstream myfileOut;
  myfileOut.open("example.txt");
  if (!myfileOut.is_open()){
    cout << "Failed to open file!\n";
    return 1;
  }
  myfileOut << "Writing this to a file.\n";
  myfileOut.close();
  
  // READ
  string line;
  ifstream myfileIn("example.txt");
  if (!myfileIn.is_open()){
    cout << "Failed to open file!\n";
    return 1;
  }
  while ( getline(myfileIn, line) ){
    cout << line << '\n';
  }
  myfileIn.close();

  return 0;
}
```
</details>
