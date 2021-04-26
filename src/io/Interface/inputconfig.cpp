//C++
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
// McCAD
#include "inputconfig.hpp"

McCAD::IO::InputConfig::InputConfig(std::string& currentPath)
    : currentPath{currentPath}{
}

McCAD::IO::InputConfig::~InputConfig(){
}

void
McCAD::IO::InputConfig::writeTemplate(){
    std::ofstream inputConfig;
    inputConfig.open("McCADInputConfig.txt");
    inputConfig << "# McCAD Run Parameters\n"
                   "# ====================\n" << std::endl;
    inputConfig << "# Input\n"
                   "# =====\n"
                   "units = cm\n"
                   "# > Path to the input STEP file;\n"
                   "inputFileName = input.stp\n" << std::endl;
    inputConfig << "# Decomposition\n"
                   "# ===================\n"
                   "decompose = true\n"
                   "# > Desired name of the decomposed solids output STEP file;\n"
                   "resultFileName = result.stp\n"
                   "# > Desired name of the rejected solids output STEP file;\n"
                   "rejectFileName = reject.stp\n"
                   "# > Other parameters;\n"
                   "recurrenceDepth = 20\n"
                   "tolerance = 1.0e-7\n"
                   "minInputSolidVol = 1.0\n"
                   "angleTolerance = 1.0e-3\n" << std::endl;
    inputConfig << "# Conversion\n"
                   "# ==================\n"
                   "convert = false\n"
                   "voidGeneration = true\n"
                   "maxSolidsPerVoidCell = 20\n"
                   "BVHVoid = true\n"
                   "MCcode = mcnp\n"
                   "startCellNum = 0\n"
                   "startSurfNum = 0\n" << std::endl;
    inputConfig.close();
}

void
McCAD::IO::InputConfig::readTemplate(){
    std::ifstream inputConfig("McCADInputConfig.txt");
    if (!inputConfig){
        std::cout << "McCADInputConfig.txt is missing!. Proceeding with default "
                     "parameters:\n"
                     "Input = " << inputFileName <<
                     "\nResult = " << resultFileName << 
                     "\nReject = " << rejectFileName << std::endl;
    } else {
        // Read file and populate parameters
        while (!inputConfig.eof()){
            std::string line;
            getline(inputConfig, line);
           std::vector<std::string> lineSplit = splitLine(line, ' ');
           if (lineSplit.size() == 0 || lineSplit[0] == "#") continue;
           else {
               if (lineSplit[0] == "inputFileName")
                   inputFileName = lineSplit[2];
               else if (lineSplit[0] == "decompose")
                   decompose = lineSplit[2] == "true" ? true : false;
               else if (lineSplit[0] == "resultFileName")
                   resultFileName = lineSplit[2];
               else if (lineSplit[0] == "rejectFileName")
                   rejectFileName = lineSplit[2];
               else if (lineSplit[0] == "recurrenceDepth")
                   recurrenceDepth = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "convert")
                   convert = lineSplit[2] == "false" ? false : true;
               else if (lineSplit[0] == "voidGeneration")
                   voidGeneration = lineSplit[2] == "true" ? true : false;
               else if (lineSplit[0] == "maxSolidsPerVoidCell")
                   maxSolidsPerVoidCell = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "BVHVoid")
                   BVHVoid = lineSplit[2] == "true" ? true : false;
               else if (lineSplit[0] == "MCcode")
                   MCcode = lineSplit[2];
               else if (lineSplit[0] == "startCellNum")
                   startCellNum = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "startSurfNum")
                   startSurfNum = std::stoi(lineSplit[2]);
               else continue;
           }
        }
    }
}

std::vector<std::string>
McCAD::IO::InputConfig::splitLine(const std::string& line, char delimiter){
    std::istringstream ss(line);
    std::vector<std::string> lineSplit;
    std::string s;
    while (std::getline(ss, s, delimiter)){
        lineSplit.push_back(s);
    }
    return lineSplit;
}
