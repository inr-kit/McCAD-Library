McCAD: a CAD to Monte Carlo conversion library
----------------------------------------------
McCAD is a library for the conversion of CAD solid models into the input syntax of a variety of Monte Carlo codes; from Boundary Representation "BREP" to Constructive Solid Geometry "CSG".
The library is written in C++ and consists of two main algorithms:

1- Decompostion:
   * carries out tasks pertinent to the simplification of complex solids by means of decomposition into their primitive convex subsolids.
2- Void generation & Conversion:
   * carries out tasks pertinent to the generation of void cells and conversion of the primitive solids into MCNP input syntax, and later on to other MC codes.

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
1- CMake (version `3.25.2 <https://cmake.org/download/>`_):
   * the standard build system for McCAD library.

2- Boost C++ Libraries (version `1.81.0 <https://www.boost.org/users/download/>`_):
   * used for multiprocessing.

3- Open CASCADE Technology (version `7.7.0 <https://dev.opencascade.org/release>`_):
   * the geometry engine used for manipulating and decomposing solids.

Installation:
------------- 
Please refer to the "Installation from Source" section in the `manual <https://github.com/inr-kit/McCAD-Library/blob/master/docs/Manual%20v1.0/McCAD_manual_v1.pdf>`_.

General notes on Usage:
-----------------------
1- The library assumes a clean CAD model, one with no intersections/overlapping.
   * while in theory the library won't report an error with intersections/overlapping as solids are processed individually, it will cause issue for void generation and conversion!
2- There are known bugs with the decomposition algorithm which are currently being investigated.
   * a list of bugs and proposed workaround can be found in the "Known issues" section in the `manual <https://github.com/inr-kit/McCAD-Library/blob/master/docs/Manual%20v1.0/McCAD_manual_v1.pdf>`_.
   
Reporting issues:
-----------------
Please don't hesitate to report any issues with running the code or errors in the manual by:
   * raising issues in the github repo.
   * sending an email to moataz.harb@kit.edu & yuefeng.qiu@kit.edu.
