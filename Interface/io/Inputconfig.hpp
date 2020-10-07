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
    std::string defaultInput = "input.stl";
    std::string defaultReject = "reject.stl";
    std::string defaultResult = "result.stl";
    std::string inputFileName, resultFileName, rejectFileName, materialFileName;
    // Decomposition
    double tolerance = 1.0e-7;
    double minInputSolidVol = 1.0;
    double angleTolerance = 1.0e-3;
    // Conversion
    bool writeCollisionFile =false;
    bool voidGeneration = true;
    double maxDiscLength = 200;
    double minDecomFaceArea = 50;
    double minVoidVol = 1.0;
    unsigned int voidDecomposeDepth = 10;
    unsigned int startCellNum = 0;
    unsigned int startSurfNum = 0;
    double XResolution = 0.001;
    double YResolution = 0.001;
    double RResolution = 0.0314;
    unsigned int maxSamplPntNum = 50;
    unsigned int minSamplPntNum = 20;
    unsigned int initNumVoidBoxes = 1;
  };
}

#endif // INPUTCONFIG_H
