#ifndef INPUTCONFIG_HPP
#define INPUTCONFIG_HPP

//C++
#include <string>
#include <vector>
#include <cmath>

namespace McCAD::IO{
class InputConfig{
public:
    InputConfig(std::string& currentPath);
    ~InputConfig();

    std::string currentPath;
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
    double minSolidVolume = 1.0;
    double minFaceArea = 1.0e-4;
    double precision = 1.0e-7;
    double parameterTolerance = 1.0e-7;
    double angularTolerance = 1.0e-3 * M_PI;
    double distanceTolerance = 1.0e-5;
    // Void generation and conversion
    bool convert = false;
    bool voidGeneration = true;
    double minVoidVolume = 1.0;
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
  };
}

#endif // INPUTCONFIG_H
