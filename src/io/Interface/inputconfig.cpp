//C++
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
// McCAD
#include "inputconfig.hpp"

McCAD::IO::InputConfig::InputConfig(){}

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
                   "recurrenceDepth = 20\n"
                   "minSolidVolume = 1.0e-3 [cm3]\n"
                   "minFaceArea = 1.0e-4 [cm2]\n"
                   "scalingFactor = 100.0\n"
                   "precision = 1.0e-6\n"
                   "faceTolerance = 1.0e-8 [cm]\n"
                   "edgeTolerance = 1.0e-8 [cm]\n"
                   "parameterTolerance = 1.0e-8 [cm]\n"
                   "angularTolerance = 1.0e-4\n"
                   "distanceTolerance = 1.0e-6 [cm]\n"
                   "torusSplitAngle = 360.0\n"
                   "simplifyTori = false\n" << std::endl;
    inputConfig << "# Conversion\n"
                   "# ==========\n"
                   "convert = false\n"
                   "# > Choose whether or not to generate void cells;\n"
                   "voidGeneration = true\n"
                   "# > Minimum acceptable void volume shouldn;t be less than minSolidVolume;\n"
                   "minVoidVolume = 1.0 [cm3]\n"
                   "# > A larger number will result in fewer void cells but longer cell expressions;\n"
                   "maxSolidsPerVoidCell = 20\n"
                   "# > Choose whether or not to generate Bound Volume Heirarchy void cells;\n"
                   "BVHVoid = false\n"
                   "# > Choose the desired MC code for conversion;\n"
                   "MCcode = mcnp\n"
                   "startCellNum = 1\n"
                   "startSurfNum = 1\n"
                   "startMatNum = 1\n"
                   "maxLineWidth = 80\n"
                   "MCOutputFileName = MCFile.inp\n"
                   "volumesFileName = volumes.txt\n" << std::endl;
    inputConfig.close();
}

void
McCAD::IO::InputConfig::readTemplate(){
    std::ifstream inputConfig("McCADInputConfig.txt");
    if (!inputConfig){
        inputFileNames.push_back("input.stp");
        std::cout << "[WARNING] McCADInputConfig.txt is missing!, proceeding with default parameters:"
                     "\nInput  = input.stp" <<
                     "\nResult = input_decomposed.stp" <<
                     "\nReject = input_rejected.stp" << std::endl;
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
                   if (units == "cm") conversionFactor = 10.0;
                   else if (units == "m") conversionFactor = 1000.0;
               } else if (lineSplit[0] == "inputFileName"){
                   for(int i = 2; i < lineSplit.size(); ++i){
                       inputFileNames.push_back(lineSplit[i]);
                   }
               // Decompositions
               } else if (lineSplit[0] == "decompose")
                   decompose = stringToLowerCase(lineSplit[2]) == "true" ? true : false;
               else if (lineSplit[0] == "recurrenceDepth")
                   recurrenceDepth = std::stoi(lineSplit[2]);
               else if (lineSplit[0] == "minSolidVolume")
                   minSolidVolume = std::stof(lineSplit[2]) * std::pow(conversionFactor, 3);
               else if (lineSplit[0] == "minFaceArea")
                   minFaceArea = std::stof(lineSplit[2]) * std::pow(conversionFactor, 2);
               else if (lineSplit[0] == "scalingFactor")
                   scalingFactor = std::stof(lineSplit[2]);
               else if (lineSplit[0] == "precision")
                   precision = std::stof(lineSplit[2]);
               else if (lineSplit[0] == "faceTolerance")
                   faceTolerance = std::stof(lineSplit[2]) * conversionFactor;
               else if (lineSplit[0] == "edgeTolerance")
                   edgeTolerance = std::stof(lineSplit[2]) * conversionFactor;
               else if (lineSplit[0] == "parameterTolerance")
                   parameterTolerance = std::stof(lineSplit[2]) * conversionFactor;
               else if (lineSplit[0] == "angularTolerance")
                   angularTolerance = std::stof(lineSplit[2]) * PI;
               else if (lineSplit[0] == "distanceTolerance")
                   distanceTolerance = std::stof(lineSplit[2]) * conversionFactor;
               else if (lineSplit[0] == "torusSplitAngle")
                   torusSplitAngle = std::stof(lineSplit[2]) * PI / 180.0;
               else if (lineSplit[0] == "simplifyTori")
                   simplifyTori = stringToLowerCase(lineSplit[2]) == "true" ? true : false;
               // Conversion
               else if (lineSplit[0] == "convert")
                   convert = stringToLowerCase(lineSplit[2]) == "false" ? false : true;
               else if (lineSplit[0] == "voidGeneration")
                   voidGeneration = stringToLowerCase(lineSplit[2]) == "true" ? true : false;
               else if (lineSplit[0] == "minVoidVolume")
                   minVoidVolume = std::stof(lineSplit[2]) * std::pow(conversionFactor, 3);
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
               else if (lineSplit[0] == "startMatNum")
                   startMatNum = std::stoi(lineSplit[2]);
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
    if (decompose) populateNamesLists();
    if(convert) populateMatList();
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

void
McCAD::IO::InputConfig::populateNamesLists(){
    for(int i = 0; i < inputFileNames.size(); ++i){
        std::string splitName = splitLine(inputFileNames[i], '.')[0];
        decomposedFileNames.push_back(splitName + std::string("_decomposed.stp"));
        rejectedFileNames.push_back(splitName + std::string("_rejected.stp"));
    }
}

void
McCAD::IO::InputConfig::populateMatList(){
    for(int i = 0; i < inputFileNames.size(); ++i){
        std::string splitName = splitLine(inputFileNames[i], '.')[0];
        rejectedConvFileNames.push_back(splitName + std::string("_rejectedConv.stp"));
        std::string matName = stringToLowerCase(splitLine(splitName, '_')[0]);
        double matDensity{0.0};
        if(matName != "void" && splitLine(splitName, '_').size() > 1)
            matDensity = std::stof(splitLine(splitName, '_')[1]);
        materialsInfo.push_back(std::make_tuple(matName, matDensity));
    }
}
