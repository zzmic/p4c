<!--!
\page p4tools P4Tools                                                               
-->
<!-- 
Documentation Inclusion:
This README is integrated as a standalone page in the P4 compiler documentation.

Refer to the full page here: [P4Tools](https://p4lang.github.io/p4c/p4tools.html)
-->
<!--!
\internal
-->
# P4Tools 
<!--!
\endinternal
-->

<!--!
[TOC]
-->
P4Tools is a collection of tools that make testing P4 targets and programs a little easier. So far the platform supports the following tools and projects:

- [P4Testgen](https://github.com/p4lang/p4c/tree/main/backends/p4tools/modules/testgen): An input-output test case generator for P4.

- [P4Smith](https://github.com/p4lang/p4c/tree/main/backends/p4tools/modules/smith): A random P4 program generator in the spirit of Csmith.


## Directory Structure

```
p4tools
 ├─ cmake            ── common P4Tools CMake modules.
 ├─ common           ── common code for the various P4Tools modules.
 │  ├─ compiler      ── transformation passes for P4 code.
 │  ├─ control_plane ── code concerning P4Tool's control plane semantics.
 │  ├─ core          ── definitions for core parts of the P4Tools modules.
 │  └─ lib           ── helper functions and utilities for P4Tools modules.  
 └─  modules         ── P4Tools extensions.
    ├─ smith       ── P4Smith: a random P4 program generator.
    └─ testgen       ── P4Testgen: a test-case generator for P4 programs.
```

## Building
Please see the general installation instructions [here](https://github.com/p4lang/p4c#installing-p4c-from-source). P4Tools can be built using the following CMAKE configuration in the P4C repository.

```
mkdir build
cd build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DENABLE_TEST_TOOLS=ON
make
```

## Dependencies
* [z3](https://github.com/Z3Prover/z3) SMT solver to compute path constraints.
    * Important: We currently only support Z3 versions 4.8.14 to 4.12.0.

These dependencies are automatically installed via CMakelist's [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) module.

## Development Style
Currently, each C++ source directory has a few subdirectories, including:
* `core`, containing the core functionality of the submodule; and
* `lib`, containing supporting data structures.

The distinction between the two can be fuzzy. Here are some guiding principles
for where to find/put what:
* If it depends on anything in `core`, it belongs in `core`.
* If it's something that resembles a general-purpose data structure (e.g., an
  environment or a symbol table), it's probably in `lib`.


### C++ Coding style

P4Tools in general follows the [P4C coding style](https://github.com/p4lang/p4c/blob/main/docs/README.md#coding-conventions). Some deviations from the Style Guide are highlighted below.

* Comments are important. The Style Guide's [section on comments](https://google.github.io/styleguide/cppguide.html#Comments) is
  required reading.
    * Classes, methods, and fields are documented with triple-slash
      Doxygen-style comments:
      ```
      /// An example class demonstrating a documentation comment.
      class C {};
      ```
    * We do not use copyright headers or license boilerplate in our source
      files. Where needed, these will be auto-generated during release
      packaging.
* Generally prefer a single class declaration per `.h` file, unless providing a
  library of related classes. Multiple classes may be declared in a `.cpp`
  file.

<!--! 
\include{doc} "../backends/p4tools/CONTRIBUTORS.md"
-->
