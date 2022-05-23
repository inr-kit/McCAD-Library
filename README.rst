McCAD v1.0
----------
McCAD is a library for the conversion of CAD solid models to MCNP input syntax, from Boundary Representation "BREP" to Constructive Solid Geometry "CSG".
The library is written in C++ and consists of two main algorithms:

1- Decompostion:
   * carries tasks related to the simplification of complex solids by means of decomposition into primitive convex solids.
2- Void generation and Conversion:
   * the main responsibility is the generation of void cells and conversion of the primitive solids into MCNP input syntax, and later on to other MC codes.

General Notes:
--------------
* The latest working version of McCAD is McCAD-SALOME:
   * docs  : https://github.com/inr-kit/McCad-Salome-Docs
   * source: https://github.com/inr-kit/McCad-Salome-Source
   * binary: https://github.com/inr-kit/McCad-Salome-Binaries
 
* The current decomposition algorithm is an isolated and improved algorithm based on the latest updates/changes:
   * source: https://github.com/McCadKIT/McCad

Dependencies:
--------------
1- CMake (version 3.23.0): https://cmake.org/download/
   * The standard build system for McCAD library.

2- Boost C++ Library (Version 1.78.0): https://www.boost.org/
   * Used for multiprocessing.

3- Open CASCADE Technology (version 7.5.0): https://dev.opencascade.org/release/previous
   * The geometry engine used for manipulating and decomposing solids.

Installation:
-------------
Please refer to "Installation from Source" section in the manual in docs/Manual v1.0/McCAD_manual_v1.pdf.

General notes on Usage:
-----------------------
1- The library assumes a clean CAD model, one with no intersections or overlapping.
   * While in theory the library won't report an error with intersections/overlappings as solids are processed individually, it will cause problems for void generation and conversion later on.
2- There are known bugs with the decomposition algorithm which are currently being investigated.
   * A list of bugs and proposed fixes can be found in the docs/Manual v1.0/McCAD_manual_v1.pdf.
   
Reporting issues:
-----------------
Please, don't hesitate to report any issues with running the code or errors in the manual by:
   * Raising issues in the github repo.
   * sending an email to moataz.harb@kit.edu.
