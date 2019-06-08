# modfcn: C++-native dlfcn

This library is provided in two versions:
* a traditional header version, the one currently in active development;
* a C++ TS module version, which typically follows the header version.

Each has its own makefile and example suite.

## Building and testing

A few test executables and test modules are provided.

Be sure to have a working Clang and LLVM installation in your header and library search paths.

Build with:

`make clean && make`

Test with:

`./test-n args`

in the directory of the relevant version, where n is any number currently implemented.
