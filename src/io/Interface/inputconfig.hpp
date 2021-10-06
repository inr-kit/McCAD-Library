#ifndef INPUTCONFIG_HPP
#define INPUTCONFIG_HPP

//C++
#include <string>
#include <vector>
#include <cmath>
#include <filesystem>

namespace McCAD::IO{
  class InputConfig{
  public:
      InputConfig();
      InputConfig(const std::filesystem::path& currentPath);
      ~InputConfig();

      // Internal variables
      std::filesystem::path currentPath;
      double conversionFactor{1.0};
      const double PI = 3.141592653589793238463;
      std::string inputFileName{"input.stp"}, resultFileName{"inputDecomposed.stp"},
                  rejectFileName{"inputRejected.stp"}, outputFileName,
                  conversionFileName;
      std::vector<std::string> conversionFileNames;
      std::vector<std::tuple<std::string, double>> materialsInfo;
      bool readConversion = false;
      void writeTemplate();
      std::string stringToLowerCase(std::string& string);
      std::vector<std::string> splitLine(const std::string& line, char delimiter);
      void readTemplate();
      void populateNamesLists();
      void populateMatList();

      // General input
      std::string units = "cm";
      std::vector<std::string> inputFileNames, decomposedFileNames, rejectedFileNames,
                               rejectedConvFileNames;
      // Decomposition
      bool decompose = true;
      int recurrenceDepth = 20;
      double minSolidVolume = 1.0; // in mm^3
      double minFaceArea = 0.01;     // in mm^2
      double scalingFactor = 100.0;
      double precision = 1.0e-6;
      double faceTolerance = 1.0e-7;
      double edgeTolerance = 1.0e-7;
      double parameterTolerance = 1.0e-7;
      double angularTolerance = 1.0e-4 * PI;
      double distanceTolerance = 1.0e-5;
      double torusSplitAngle = 0.25 * PI;
      bool simplifyTori = true;
      // Void generation and conversion
      bool convert = false;
      bool voidGeneration = false;
      bool componentIsSingleCell = false;
      double minVoidVolume = 1000.0; // in mm^3
      int maxSolidsPerVoidCell = 20;
      bool BVHVoid = false;
      std::string MCcode = "mcnp";
      int startCellNum = 1;
      int startSurfNum = 1;
      int startMatNum = 1;
      int maxLineWidth = 80;
      std::string MCOutputFileName = "MCFile.inp";
      std::string volumesFileName = "volumes.txt";
  };
}

#endif // INPUTCONFIG_HPP
