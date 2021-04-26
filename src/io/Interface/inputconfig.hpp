#ifndef INPUTCONFIG_HPP
#define INPUTCONFIG_HPP

//C++
#include <string>
#include <vector>

namespace McCAD::IO{
class InputConfig{
public:
    InputConfig(std::string& currentPath);
    ~InputConfig();

    std::string currentPath;
    void writeTemplate();
    std::vector<std::string> splitLine(const std::string& line, char delimiter);
    void readTemplate();

    //Define default values
    std::string units = "cm";
    std::string inputFileName = "input.stp";
    std::string resultFileName = "result.stp";
    std::string rejectFileName = "reject.stp";
    // Decomposition
    bool decompose = true;
    int recurrenceDepth = 20;
    double tolerance = 1.0e-7;
    double minInputSolidVol = 1.0;
    double angleTolerance = 1.0e-3;
    // Void generation and conversion
    bool convert = false;
    bool voidGeneration = true;
    int maxSolidsPerVoidCell = 20;
    bool BVHVoid = true;
    std::string MCcode = "mcnp";
    int startCellNum = 0;
    int startSurfNum = 0;

    // Internal variables
    std::string conversionFileName = "conversion.stp";
  };
}

#endif // INPUTCONFIG_H
