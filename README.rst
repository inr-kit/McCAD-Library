McCAD v1.0
-----------
McCAD is an interface library for the conversion of CAD solid models, Boundary Representation "BRep", to MCNP, Constructive Solid Geometry "CSG".
The library is written in C++ and consists of two main algorithms:

1- Decompostion:
   * carries tasks related to the simplification of complex solids by means of decomposition into primitive convex solids.
2- Void generation and Conversion:
   * the main responsibility is the generation of void cells and conversion of the primitive solids into MCNP input syntax, and later on to other MC codes.

Decomposition:
--------------
* The latest working version of McCAD is McCAD-SALOME:
   * docs  : https://github.com/inr-kit/McCad-Salome-Docs
   * source: https://github.com/inr-kit/McCad-Salome-Source
   * binary: https://github.com/inr-kit/McCad-Salome-Binaries
 
* The current decomposition algorithm is an isolated and improved algorithm based on the latest updates/changes:
   * source: https://github.com/McCadKIT/McCad

Prerequisites:
--------------
1- CMake (version 3.23.0): https://cmake.org/download/
   * The standard build system for McCAD library.

2- Boost C++ Library (Version 1.78.0): https://www.boost.org/
   * Used for multiprocessing.

3- Open CASCADE Technology (version 7.5.0): https://dev.opencascade.org/release
   * The geometry engine used for manipulating and decomposing solids.

Installation (Linux: Ubuntu20.04.3LTS):
---------------------
1- CMake
   * Download cmake-3.23.0.tar.gz from https://cmake.org/download/ and then run the commands below in a terminal.
   * $ tar -xzvf cmake-3.23.0.tar.gz
   * $ cd cmake-3.23.0
   * $ mkdir build
   * $ cd build
   * $ cmake .. -DCMAKE_USE_OPENSSL=OFF -DCMAKE_INSTALL_PREFIX=.
   * $ make && make install

2- Boost C++ lirary
   * Download boost_1_78_0.tar.gz from https://www.boost.org/users/download/ and then run the commands below in a terminal.
   * $ tar -xvzf boost_1_78_0.tar.gz
   * $ cd boost_1_78_0
   * $ mkdir build
   * $ cd tools/build
   * $ ./bootstrap.sh
   * $ ./b2 install --prefix=../../build/

3- Open CASCADE Technology
   * NOTE: instructions on the installation of dependencies can be found in https://dev.opencascade.org/doc/occt-7.5.0/overview/html/index.html#intro_install_windows
   * Download opencascade-7.5.0.tgz from https://dev.opencascade.org/release/previous and then run the commands below in a terminal.
   * $ tar -xzvf opencascade-7.5.0.tgz
   * $ cd opencascade-7.5.0
   * $ mkdir build
   * $ cd build
   * $ cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_LIBRARY_TYPE=Shared -DCMAKE_INSTALL_PREFIX=. -DINSTALL_TEST_CASES=TRUE -DINSTALL_DOC_Overview=TRUE
   * $ make && make install

4- McCAD
   * NOTE: building a shared library is recommended!. Should a static library be needed, the user has to insure a compliant installation of Open CASCADE Technology by changing the byuld type, -DBUILD_LIBRARY_TYPE=STATIC.
   * $ git clone https://github.com/moatazharb/McCAD_refactor
   * $ cd McCAD_refactor
   * $ mkdir build
   * $ cd build
   * $ CMake .. -DCMAKE_INSTALL_PREFIX=. -DBUILD_STATIC=OFF -DBOOST_CUSTOM_ROOT=<PATH to boost_1_78_0> -DOCC_CUSTOM_ROOT=<PATH to opencascade-7.5.0/build_shared> -DBUILD_RPATH=ON
   * $ make && make install

Installation (Windows 10):
--------------------------
1- Install CMake, Boost, and OCCT
   * Same steps 1 - 3 can be followed by downloading the install files that is suitable for Windows 10
2- Install McCAD
   * Building a static executable is recommended.
   * Path to OCC should be provided through -DOCC_CUSTOM_ROOT=<path to OCC> CMake flag.
   * Path to Boost should be provided though -DBOOST_CUSTOM_ROOT=<path to Boost> CMake flag.

General notes on Usage:
-----------------------
1- The library assumes a clean CAD model (one with no intersections or overlapping).
   While in theory the library won't report an error with intersections/overlappings as solids are processed individually,
   it will cause problems for void generation and conversion later on.
2- There are known issues with the decomposition algorithm which are currently being investigated. Those issues include:
   * A list of issues and possible fixes can be found in the manual.
