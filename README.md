
# Build Instructions

`bash build.sh` # using cmake generates compiler specific project files  
`cd build`  
`./SLBackend` # run the compiler, compiles the input.txt and generates output.asm  

`~/vicharak/8bit-computer/asm/asm.py ./output.asm > ~/vicharak/8bit-computer/memory.list` # run the assembler  
`cd ~/vicharak/8bit-computer`  
`make clean && make run` # run the executable 

