McCAD: CAD to Monte Carlo conversion library
--------------------------------------------
McCAD is a library for the conversion of CAD solid models to the input syntax of a variety of Monte Carlo codes; from Boundary Representation "BREP" to Constructive Solid Geometry "CSG".
The library is written in C++ and consists of two main algorithms:

1- Decompostion:
   * carries out tasks related to the simplification of complex solids by means of decomposition into primitive convex subsolids.
2- Void generation and Conversion:
   * carries out tasks related to the generation of void cells and conversion of the primitive solids into MCNP input syntax, and later on to other MC codes.

Useful links:
-------------
* McCAD-SALOME: McCAD library integrated with the `Salome <https://www.salome-platform.org/>`_ platform:
   * `docs <https://github.com/inr-kit/McCad-Salome-Docs>`_
   * `source <https://github.com/inr-kit/McCad-Salome-Source>`_
   * `binaries <https://github.com/inr-kit/McCad-Salome-Binaries>`_
 
* McCAD-FreeCAD: an improved McCAD library integrated with the opensource CAD software `FreeCAD <https://www.freecadweb.org/>`_:
   * `source <https://github.com/inr-kit/McCAD-FreeCAD>`_
   
* csg2csg: a tool for the translation between a variety of Monte Carlo code formats: 
   * `source <https://github.com/makeclean/csg2csg>`_.

Dependencies:
--------------
1- CMake (version `3.23.0 <https://cmake.org/download/>`_):
   * the standard build system for McCAD library.

2- Boost C++ Library (version `1.78.0 <https://www.boost.org/>`_):
   * used for multiprocessing.

3- Open CASCADE Technology (version `7.5.0 <https://dev.opencascade.org/release/previous>`_):
   * the geometry engine used for manipulating and decomposing solids.

Installation:
-------------
Please refer to the "Installation from Source" section in the `manual <https://github.com/moatazharb/McCAD/blob/develop/docs/Manual%20v1.0/McCAD_manual_v1.pdf>`_.

General notes on Usage:
-----------------------
1- The library assumes a clean CAD model, one with no intersections or overlapping.
   * while in theory the library won't report an error with intersections/overlappings as solids are processed individually, it will cause problems for void generation and conversion later on.
2- There are known bugs with the decomposition algorithm which are currently being investigated.
   * a list of bugs and proposed fixes can be found in the "Known issues" section in the `manual <https://github.com/moatazharb/McCAD/blob/develop/docs/Manual%20v1.0/McCAD_manual_v1.pdf>`_.
   
Reporting issues:
-----------------
Please don't hesitate to report any issues with running the code or errors in the manual by:
   * raising issues in the github repo.
   * sending an email to moataz.harb@kit.edu.