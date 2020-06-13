McCAD v1.0L
----------------------
McCAD is an interface library that aides in the conversion of CAD solid models "BRep" to MCNP "CSG".
The library written in C++ and is decomposed into three main parts:
- Decompostion:
    - carries tasks related to simplification of complex solid models by means of decomposition
    into primitive convex solids.
- Void generation and Conversion (coming to this repo by end of year):
    - the main responsibility is the generation of void and conversion of the resulting primitive
    solids into MCNP input syntax.
- Material (coming to this repo by end of year): 
    - maps material definitions to resulting decomposed solids to be written to MCNP input format.

Decomposition
-------------
The last working version of McCAD is McCAD-SALOME (source: https://github.com/moatazharb/McCad-Salome-Source,
exec.: https://github.com/moatazharb/McCad-Salome-Binaries). 
The current library is an isolated and improved algorithm based on the latest updates/changes implemented in
(https://github.com/McCadKIT/FreeCAD-McCad).

Building the library:
Prerequisites:
--------------
1- OpenCascade                  : https://www.opencascade.com/content/latest-release
  - Geometry engine for manipulating and decomposing solids.
  
2- CMake (version 3.14 or above): https://cmake.org/download/
  - Standard build system for McCAD v1.0L library.
  
3- Boost C++ Library: https://www.boost.org/
  - Used for multiprocessing.

Installation:
------------
A- Linux
  -> Install CMake
    - Download CMake tar file https://github.com/Kitware/CMake/releases/download/v3.17.3/cmake-3.17.3.tar.gz
    - tar -xzvf cmake-3.17.3.tar.gz
    - cd cmake-3.17.3
    - mkdir build
    - cd build
    - cmake .. -DCMAKE_USE_OPENSSL=OFF && make && make install
  -> Install Boost C++ lirary
    - Download boost_1_73_0.tar.gz from https://dl.bintray.com/boostorg/release/1.73.0/source/
    - tar -xvzf boost_1_73_0.tar.gz
    - cd /tools/build
    - ./bootstrap.sh
    - ./b2 install --prefix=<Dir to install in>
  -> Install OpenCascade
    - Download opencascade-7.4.0.tgz from https://www.opencascade.com/content/latest-release or from
      https://www.opencascade.com/content/latest-release if new releases exist by the time of installation
    - 
    

General notes on Usage:
-------------
1- The library assumes a clean CAD model (one with no intersections or overlapping). While in theory the library
   won't report an error with intersections/overlappings as solids are processed individually, it will cause problems
   for void generation and conversion later on.
   
2- The heirarchy of solids (groups/subgroups in CAD) is not preserced in the current version. This means that the output
   STEP file will have a flat heirarchy of sloids with all info about groups/subgroups lost.
   - A good practice to circumvent this in the current version is to split the input CAD solid into several files by material.
   This will result in output file of decomposed solids of a single material to be assigned later on when converting.
   
3- Void generation and conversion is yet to be implemented in the current library. While the current library will provide
   advanced and better decomposition compared to the McCAD-SALOME version, McCAD-SALOME is to be used for conversion, for now.