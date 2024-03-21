# Library
A library is a group of pre-compiled coded, called functions, to avoid repetition of code, package is created, called library.


## Static Library

A **static library** is a collection of object files that are linked together during the compilation phase of a program.

### Building Static Library

#### 1. code  for the library

source code to create a simple library named libCalc

- Addition.c
- Subtracion.c
- division.c
- subtraction.c
- modulus.c
 
#### 2. Compile the Source Files

compile the source files into .o(object) files

```bash
gcc -c code/Addition.c -o lib/Addition.o
gcc -c code/subtraction.c -o lib/subtraction.o
gcc -c code/division.c -o lib/division.o
gcc -c code/multiplication.c -o lib/multiplication.o
gcc -c code/modulus.c -o lib/modulus.o
```

#### 3. Create the Static Library

Create the static library (myMath.a) and add the object filesto it.
Use the `ar` (archiver) command to create the static library (myMath.a):
```bash
ar rcs myMath.a lib/*.o
```
#### 4. Use the library in the Program

use the static library in the prograam --> Create a program (main.c) and include the header
file of the library to it and calls the functions of the library


#### 5. Compile the Program with the Static Library

```bash
gcc main.c -static -o my_program -I include/ -L. lib/myMath.a
```
`-static` tells the compiler to compile statically
`-I` tells the compiler where to find the header file
`-L` tells the linker to look for libraries in the current directory


#### 6. Run the Program

Execute the compiled program
```bash
./my_program
```


## Dynamic Library

A dynamic library is a collection of compiled code that can be loaded into a program at runtime

### Building a Dynamic Library

#### 1. code  for the library

source code to create a simple library named libCalc

- Addition.c
- Subtracion.c
- division.c
- subtraction.c
- modulus.c


#### 2. Compile the Dynamic Library

compile the source files into .so(shared object) files

```bash
gcc -shared -fPIC -o lib/libmyMath.so code/*.c
```
The `-shared` flag indicates that you are creating a shared object, and `-fPIC` generates position-independent code, which is necessary for shared libraries.

#### 3. Use the library in the Program

use the Dynamic library in the prograam --> Create a program (main.c) and include the header
file of the library to it and calls the functions of the library


#### 4. Compile the Program with the Dynamic Library

```bash
gcc main.c -o my_program -I include -L./lib -lmyMath -Wl,-rpath=./lib
```
`-I` tells the compiler where to find the header file, `-L` tells the linker to look for libraries in the current directory, '-lmylib' links your program with libmylib.so, and `-Wl,-rpath=./lib` tells the sysytem loader where to search for the shared object file


#### 5. Run the Program

Execute the compiled program
```bash
./my_program
```




 






