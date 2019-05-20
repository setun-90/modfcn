# modfcn: dlfcn equivalent for C++

This library is provided in two versions:
* a traditional header version, the one currently in active development;
* a C++ TS module version, which typically follows the header version.

Each has its own makefile and example suite.

## Building and testing

A few test executables and test modules are provided.

Building is as simple as:

`make clean && make`

Testing is performed simply by executing

`./test-n args`

in the directory of the relevant version, where n is any number currently implemented.

## TODO

* Out-of-tree builds.
* Separate test modules from test executables.
