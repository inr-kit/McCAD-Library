# McCAD Run Parameters    Mon May 23 17:10:46 2022
# ====================

# Input
# =====
# > Debug level: 0, 1, 2, 3. [0] provides no debugging outputs.
debugLevel = 0
# > The unit used for the input STEP file(s).
units = cm
# > Path to the input STEP file;
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
# > Choose whether or not to generate void cells;
voidGeneration = false
# > Condition to treat a compound as a single cell or a group of cells.
compoundIsSingleCell = false
# > Minimum acceptable void volume shouldn;t be less than minSolidVolume;
minVoidVolume = 1.0 [cm3]
# > A larger number will result in fewer void cells but longer cell expressions;
maxSolidsPerVoidCell = 40
# > Choose whether or not to generate Bound Volume Heirarchy void cells;
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

