//C++
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
// McCAD
#include "inputconfig.hpp"

McCAD::IO::InputConfig::InputConfig(const std::filesystem::path& currentPath)
    : currentPath{currentPath}{
}

McCAD::IO::InputConfig::~InputConfig(){
}

void
McCAD::IO::InputConfig::writeTemplate(){
    std::ofstream inputConfig;
    std::filesystem::path templateName = currentPath / "McCADInputConfig.txt";
    inputConfig.open(templateName.string());
    inputConfig << "# McCAD Run Parameters\n"
                   "# ====================\n" << std::endl;
    inputConfig << "# Input\n"
                   "# =====\n"
                   "units = cm\n"
                   "# > Path to the input STEP file;\n"
                   "inputFileName = input.stp\n" << std::endl;
    inputConfig << "# Decomposition\n"
                   "# =============\n"
                   "decompose = true\n"
                   "# > Desired name of the decomposed solids output STEP file;\n"
                   "resultFileName = result.stp\n"
                   "# > Desired name of the rejected solids output STEP file;\n"
                   "rejectFileName = reject.stp\n"
                   "# > Other parameters;\n"
                   "recurrenceDepth = 20\n"
                   "minSolidVolume = 1.0\n"
                   "minFaceArea = 1.0\n"
                   "precision = 1.0e-7\n"
                   "parameterTolerance = 1.0e-7\n"
                   "angularTolerance = 1.0e-3\n"
                   "distanceTolerance = 1.0e-5\n" << std::endl;
    inputConfig << "# Conversion\n"
                   "# ==========\n"
                   "convert = false\n"
                   "# > Choose whether or not to generate void cells;\n"
                   "voidGeneration = true\n"
                   "# > Minimum acceptable void volume shouldn;t be less than minSolidVolume;\n"
                   "minVoidVolume = 1.0\n"
                   "# > A larger number will result in fewer void cells but longer cell expressions;\n"
                   "maxSolidsPerVoidCell = 20\n"
                   "# > Choose whether or not to generate Bound Volume Heirarchy void cells;\n"
                   "BVHVoid = true\n"
                   "# > Choose the desired MC code for conversion;\n"
                   "MCcode = mcnp\n"
                   "startCellNum = 1\n"
                   "startSurfNum = 1\n"
                   "maxLineWidth = 80\n"
                   "MCOutputFileName = MCFile.inp\n"
                   "volumesFileName = volumes.txt" << std::endl;
    inputConfig.close();
}

void
McCAD::IO::InputConfig::readTemplate(){
    std::ifstream inputConfig("McCADInputConfig.txt");
    if (!inputConfig){
        std::cout << "McCADInputConfig.txt is missing!. Proceeding with default parameters:"
                     "\nInput  = " << inputFileName <<
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
               // General input.
               if (lineSplit[0] == "units"){
                   units = stringToLowerCase(lineSplit[2]);
                   if (units == "cm") conversion_factor = 10;
                   else if (units == "m") conversion_factor = 1000;
               } else if (lineSplit[0] == "inputFileName")
                   inputFileName = lineSplit[2];
               // Decompositions
               else if (lineSplit[0] == "decompose")
                   decompose = stringToLowerCase(lineSplit[2]) == "true" ? true : false;
               else if (lineSplit[0] == "resultFileName")
                   resultFileName = lineSplit[2];
               else if (lineSplit[0] == "rejectFileName")
                   rejectFileName = lineSplit[2];
               else if (lineSplit[0] == "recurrenceDepth")
                   recurrenceDepth = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "minSolidVolume")
                   minSolidVolume = std::stof(lineSplit[2]) * conversion_factor;
               else if (lineSplit[0] == "minFaceArea")
                   minFaceArea = std::stof(lineSplit[2]) * conversion_factor;
               else if (lineSplit[0] == "precision")
                   precision = std::stof(lineSplit[2]);
               else if (lineSplit[0] == "parameterTolerance")
                   parameterTolerance = std::stof(lineSplit[2]);
               else if (lineSplit[0] == "angularTolerance")
                   angularTolerance = std::stof(lineSplit[2]) * PI;
               else if (lineSplit[0] == "distanceTolerance")
                   distanceTolerance = std::stof(lineSplit[2]);
               // Conversion
               else if (lineSplit[0] == "convert")
                   convert = stringToLowerCase(lineSplit[2]) == "false" ? false : true;
               else if (lineSplit[0] == "voidGeneration")
                   voidGeneration = stringToLowerCase(lineSplit[2]) == "true" ? true : false;
               else if (lineSplit[0] == "minVoidVolume")
                   minVoidVolume = std::stof(lineSplit[2]) * conversion_factor;
               else if (lineSplit[0] == "maxSolidsPerVoidCell")
                   maxSolidsPerVoidCell = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "BVHVoid")
                   BVHVoid = stringToLowerCase(lineSplit[2]) == "true" ? true : false;
               else if (lineSplit[0] == "MCcode")
                   MCcode = stringToLowerCase(lineSplit[2]);
               else if (lineSplit[0] == "startCellNum")
                   startCellNum = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "startSurfNum")
                   startSurfNum = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "maxLineWidth")
                   maxLineWidth = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "MCOutputFileName")
                   MCOutputFileName = lineSplit[2];
               else if (lineSplit[0] == "volumesFileName")
                   volumesFileName = lineSplit[2];
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

std::string
McCAD::IO::InputConfig::stringToLowerCase(std::string& string){
    std::transform(string.begin(), string.end(), string.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return string;
}
