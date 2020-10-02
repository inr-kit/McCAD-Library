//C++
#include <filesystem>
#include <iostream>
#include <fstream>
// McCAD
#include "Inputconfig.hpp"

McCAD::IO::InputConfig::InputConfig(std::string& currentPath)
    : currentPath{currentPath}{
}

McCAD::IO::InputConfig::~InputConfig(){
}

void
McCAD::IO::InputConfig::writeTemplate(){
    std::ofstream inputConfig;
    inputConfig.open("McCADInputConfig.txt");
    inputConfig << "# McCAD Input Parameters\n"
                   "# ======================" << std::endl;
    inputConfig << "#> Path to the input STL file\n"
                   "inputFileName:" << std::endl;
    inputConfig << "# McCAD decomposition\n"
                   "====================="
                   "#> Desired name of the decomposed solids output STL file\n"
                   "resultFileName:\n"
                   "#> Desired name of the rejected solids output STL file\n"
                   "rejectFileName:\n"
                   "tolerance = 1.0e-7\n"
                   "minInputSolidVol = 1.0\n"
                   "angleTolerance = 1.0e-3" << std::endl;
    inputConfig << "# McCAD conversion\n"
                   "=================="
                   "writeCollisionFile = false\n"
                   "voidGeneration = true\n"
                   "maxDiscLength = 200\n"
                   "minDecomFaceArea = 50\n"
                   "minVoidVol = 1.0\n"
                   "voidDecomposeDepth = 10\n"
                   "startCellNum = 0\n"
                   "startSurfNum = 0\n"
                   "XResolution = 0.001\n"
                   "YResolution = 0.001\n"
                   "RResolution = 0.0314\n"
                   "maxSamplPntNum = 50\n"
                   "minSamplPntNum = 20\n"
                   "initNumVoidBoxes = 1" << std::endl;
    inputConfig.close();
}

void
McCAD::IO::InputConfig::readTemplate(){
    std::ifstream inputConfig("McCADInputConfig.txt");
    if (!inputConfig) std::cout << "Config file is missing!" << std::endl;
    // Read files and populate parameters
    std::string line;
    std::cout << "running" << std::endl;
    //while (!inputConfig.eof()){
    //    inputConfig.getline(inputLine);
    //    if(line.Value(1) == '#') continue;
    //   else{std::cout << line << std::endl;}
    //}
}
