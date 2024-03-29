# McCAD v1.0 run parameters / Thu Jun  9 14:27:26 2022
# =========================

# Input
# =====
# > Debugging level: 0, 1, 2, 3. [0] provides no debugging outputs.
debugLevel = 0
# > The unit used in the input STEP file(s). Only a single unit for all input STEP files!
units = cm
# > Name(s) of the input STEP file(s) separated by a space;
inputFileName = SS_-793e-2.stp

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
simplifyTori = true
simplifyAllTori = false
torusSplitAngle = 30.0 [degrees]

# Conversion
# ==========
convert = true
# > Choose whether to generate void cells;
voidGeneration = true
# > Condition to treat a compound as a single cell or a group of cells.
compoundIsSingleCell = false
# > Minimum acceptable void volume should not be less than minSolidVolume;
minVoidVolume = 1.0 [cm3]
# > A larger number will result in fewer void cells but longer cell expressions;
maxSolidsPerVoidCell = 40
# > Choose whether to generate Bound Volume Heirarchy void cells;
BVHVoid = false
# > Choose the desired MC code for conversion;
MCcode = mcnp
startCellNum = 1
startSurfNum = 1
startMatNum = 1
maxLineWidth = 80
MCFileName = MCFile.i
volumesFileName = volumes.i
voidCellsFileName = voidCells.i

