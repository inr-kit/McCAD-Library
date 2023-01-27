# McCAD v1.1 run parameters / Wed Jan 25 22:48:43 2023
# =========================

# Input
# =====
# > Debugging level: 0, 1, 2, 3. [0] provides no debugging outputs;
debugLevel = 0
# > The unit used in the input STEP file(s). Only a single unit for all input STEP files!
units = cm
# > Name(s) of the input STEP file(s) separated by a space;
inputFileName = input.stp

# Decomposition
# =============
decompose = true
recurrenceDepth = 20
minSolidVolume = 1.0e-3 [cm3]
minFaceArea = 1.0e-4 [cm2]
scalingFactor = 100.0
precision = 1.0e-6
faceTolerance = 1.0e-8 [cm]
edgeTolerance = 1.0e-8 [cm]
parameterTolerance = 1.0e-8 [cm]
angularTolerance = 1.0e-4 [radian/PI]
distanceTolerance = 1.0e-6 [cm]
simplifyTori = false
simplifyAllTori = false
torusSplitAngle = 30.0 [degrees]

# Conversion
# ==========
convert = false
# > Choose whether to generate void cells;
voidGeneration = false
# > Condition to treat a compound from the STEP file as a single cell, resulting in a unsion of subsolids cells expresseions;
compoundIsSingleCell = false
# > Minimum acceptable void volume should not be less than minSolidVolume;
minVoidVolume = 1.0 [cm3]
# > A larger number will result in fewer void cells but longer cell expressions;
maxSolidsPerVoidCell = 20
# > Choose whether to generate Bound Volume Heirarchy void cells;
BVHVoid = false
# > Choose the desired MC code for conversion;
MCcode = mcnp
startCellNum = 1
startSurfNum = 1
startMatNum = 1
maxLineWidth = 80
# Name of the MC input file to be generated;
MCFileName = MCFile.i
# Name of the Cell ID to solid volume mapping text file to be generated;
volumesFileName = volumes.i
# Name of the void to material cell IDs mapping text file to be generated;
voidCellsFileName = voidCells.i

