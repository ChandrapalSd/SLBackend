
# Introduction
SimpleLang is a minimalistic high-level language designed to run on an [8-bit CPU](https://github.com/lightcode/8bit-computer). It includes basic constructs such as variable declarations, assignments, arithmetic operations, and conditional statements, but it does not include loops. This language aims to be easy to understand and implement for educational purposes. 

## Example Program in SimpleLang 

```c
// Variable declaration 
int a;
int b; 
int c; 

// Assignment 
a = 10; 
b = 20; 
c = a + b; 

// Conditional 
if (c == 30) { 
 c = c + 1; 
} 
```

# Build Instructions

`bash build.sh` # using cmake generates compiler specific project files  
`cd build`  
`./SLBackend` # run the compiler, compiles the input.txt and generates output.asm  

`~/vicharak/8bit-computer/asm/asm.py ./output.asm > ~/vicharak/8bit-computer/memory.list` # run the assembler  
`cd ~/vicharak/8bit-computer`  
`make clean && make run` # run the executable 

<br><br><br>
###### *Disclaimer: This repository was created as part of a recruitment task for the Compiler Engineer position at [Vicharak](https://vicharak.in/).*
