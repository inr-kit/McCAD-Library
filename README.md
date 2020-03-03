McCAD v1.0L
----------------------
McCAD is an interface library that aides in the conversion of CAD solid models "BRep" to MCNP "CSG".
The library is rwritten in C++ and is decomposed into three main parts:
- Decompostion: carries tasks related to simplification of complex solid models by means of decomposition
  into primitive convex solids.
- Void generation and Conversion: the main responsibility is the generation of void and conversion of the
  resulting primitive solids into MCNP input syntax.
- Material: maps material definitions to resulting decomposed solids to be written to MCNP input format.

Decomposition
-------------
An isolated and improved algorithm based on the latest updates/changes implemented in (https://github.com/McCadKIT/FreeCAD-McCad).

Void generation and Conversion
------------------------------
coming soon

Materials
---------
Coming soon

Building the library:
Prerequisites:
--------------