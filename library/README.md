# Library
A library is a group of pre-compiled coded, called functions, to avoid repetition of code, package is created, called library.


## Static Library

### Introduction

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
gcc main.c -o my_program -I include/ -L. lib/myMath.a
```
`-I` tells the linker where to find the header file
`-L` tells the linker to look for libraries in the current directory


#### 6. Run the Program

Execute the compiled program
```bash
./my_program
```





