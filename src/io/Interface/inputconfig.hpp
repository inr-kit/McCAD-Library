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

      std::filesystem::path currentPath;
      int conversion_factor{1};
      const double PI = 3.141592653589793238463;
      void writeTemplate();
      std::vector<std::string> splitLine(const std::string& line, char delimiter);
      void readTemplate();
      std::string stringToLowerCase(std::string& string);
      //Define default values
      std::string units = "cm";
      std::string inputFileName = "input.stp";
      std::string resultFileName = "result.stp";
      std::string rejectFileName = "reject.stp";
      // Decomposition
      bool decompose = true;
      int recurrenceDepth = 20;
      double minSolidVolume = 10.0; // in mm^3
      double minFaceArea = 10.0;     // in mm^2
      double scalingFactor = 100.0;
      double precision = 1.0e-7;
      double faceTolerance = 1.0e-7;
      double edgeTolerance = 1.0e-7;
      double parameterTolerance = 1.0e-7;
      double angularTolerance = 1.0e-4 * PI;
      double distanceTolerance = 1.0e-5;
      // Void generation and conversion
      bool convert = false;
      std::string rejectConvFileName = "rejectConv.stp";
      bool voidGeneration = true;
      double minVoidVolume = 10.0; // in mm^3
      int maxSolidsPerVoidCell = 20;
      bool BVHVoid = true;
      std::string MCcode = "mcnp";
      int startCellNum = 1;
      int startSurfNum = 1;
      int maxLineWidth = 80;
      std::string MCOutputFileName = "MCFile.inp";
      std::string volumesFileName = "volumes.txt";
      // Internal variables
      std::string conversionFileName = "conversion.stp";
      std::string outputFileName;
      bool readConversion = false;
  };
}

#endif // INPUTCONFIG_H
