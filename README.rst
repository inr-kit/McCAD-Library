McCAD v1.0
-----------
McCAD is an interface library that aides in the conversion of CAD solid models, BRep, to MCNP, CSG.
The library is written in C++ and can be decomposed into three main parts:

1- Decompostion:
   * carries tasks related to simplification of complex solids by means of decomposition into
     primitive convex solids.
2- Void generation and Conversion:
   * the main responsibility is the generation of void and conversion of the resulting primitive
     solids into MCNP input syntax, and later on other MC codes.

Decomposition:
--------------
* The last working version of McCAD is McCAD-SALOME:
   * source: https://github.com/moatazharb/McCad-Salome-Source
   * binary: https://github.com/moatazharb/McCad-Salome-Binaries
 
* The current library is an isolated and improved algorithm based on the latest updates/changes:
   * source: https://github.com/McCadKIT/FreeCAD-McCad.

Prerequisites:
--------------
1- OpenCascade (OCCT version 7.5.0): https://dev.opencascade.org/release
   * Geometry engine for manipulating and decomposing solids.

2- CMake (version 3.14 or above): https://cmake.org/download/
   * Standard build system for McCAD library.

3- Boost C++ Library: https://www.boost.org/
   * Used for multiprocessing.

Installation (Linux):
---------------------
1- Install CMake
   * Download CMake tar file https://cmake.org/download/
   * tar -xzvf cmake-3.17.3.tar.gz
   * cd cmake-3.17.3
   * mkdir build
   * cd build
   * cmake .. -DCMAKE_USE_OPENSSL=OFF -DCMAKE_INSTALL_PREFIX=. && make && make install

2- Install Boost C++ lirary
   * Download boost_1_73_0.tar.gz from https://dl.bintray.com/boostorg/release/1.73.0/source/
   * tar -xvzf boost_1_73_0.tar.gz
   * cd /tools/build
   * ./bootstrap.sh
   * ./b2 install --prefix=<install Dir>

3- Install OpenCascade (OCCT)
   * Download occt-7.5.0.tgz from https://www.opencascade.com/content/latest-release
   * tar -xzvf occt-7.5.0.tgz
   * cd occt-7.5.0
   * mkdir build
   * cd build
   * cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_LIBRARY_TYPE=Shared -DCMAKE_INSTALL_PREFIX=. 
              -DINSTALL_TEST_CASES=TRUE -DINSTALL_DOC_Overview=TRUE 

4- Install McCAD
   * McCAD library and executable installation can proceed by providing the custom CMake flags.
   * Building a shared executable is recommended.

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
