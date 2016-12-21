# Things I learnt

# 21/12/16

Wrestling with horrible linker errors not letting me build an executable.

The idea below is to build different artefacts with different macros enabled. There are 4 stages of compilation:
 * Pre-processing - macros are enabled here
 * Compilation
 * Assembling
 * Linking 

Preprocessing is the stage, where macros are evaluated and set. Unfortunately, gcc doesn’t allow to redirect the output of pre-processor, but as I understand, in the olden days, this stage created .i files. 
Compilation takes the inlined .i files and turns them into assembly (.s) files. 
Assembly files are fed into the assembler to create object files, which are then linked into an executable (statically or dynamically). 

My errors happen at linker stage, but I suspect they stem from bad preprocessing and compilation.


## 19/12/16

Learnt about debug flags and how to set them at compile time. Gcc –D option takes name=value pairs and sets them to define macros. Can be used for debug functions. The optimal strategy would be to:
 
- [ ] Split debugging functions into a separate header file
- [ ] Pass flag’s value to decide the level of verbosity in debug prints 


## 6/12/16

https://vorpus.org/blog/why-does-calloc-exist/

Read about the importance of calloc vs. malloc + memset. Calloc allocates memory and sets it to 0 better than malloc + memset does. 2 main flaws – malloc + memset doesn’t check for buffer overflow and malloc + memset is slower by allocating memory twice. 

Calloc gets the memory from the kernel, which uses a virtual memory hack by mapping cope-on-write copies of a zeroed single VM page. Only when we really start writing to those pages, does the kernel have to fetch actual RAM. Lazy memory allocation – very FP-like. 
