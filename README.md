# Pomar

[![Build Status](https://travis-ci.org/dennisjosesilva/pomar.svg?branch=master)](https://travis-ci.org/dennisjosesilva/pomar)

Pomar is a library in C++ 11 to work with morphlogical trees. Its goal is to be a self-contained and easy to install library 
which provides a set of classes that abstracts common algorithms to create and handle morphological trees.

Building
------------
Pomar has generic code using templates and code with no template at all. Initially, we choose to put all code which does not use templates in implementation file. In order to keep the compilation time as low as possible for the user.

The Pomar library can be built using cmake. For terminal user, you just need follow the steps below:

1. install cmake (https://cmake.org/)
2. download pomar library
3. go to the root directory (where there is a file CMakeLists.txt)
4. run:  >$ cmake . -Bbuild
5. go to the just created directory called 'build'
6. build your project. For example, if you are working with makefile just run: >$ make
7. after build you can run the tests by running: >$ ctest
8. If you do not want to compile the tests, in the step 6, you should compile just the library, for example, run: >$ make pomar
9. after compile, pomar will generate a static library in the build directoty (libpomar), in order to integrate pomar in you project you should link this directory as well as indicate the include directory. For example, to compile in gcc you should use the following options: -std=c++11 -L${PomarDirectory}/build -I${PomarDirectory}/include -lpomar
