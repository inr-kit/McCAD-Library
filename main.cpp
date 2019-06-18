// C++
#include <iostream>
#include <string>
// McCAD
#include "McCADConfig.hpp"
#include "inputdata.hpp"
//#include "decomposition.hpp"
#include <STEPControl_Reader.hxx>
int main () {
    std::string fileName;
    std::cout << "Type a file name: " << std::endl;
    getline(std::cin, fileName);
    //McCAD::General::InputData inputData;
    //inputData.setFileName(fileName);
    //std::cout << "Loading file: " << inputData.getFileName() << std::endl;

    // read the file
    // McCAD::Decomposition::Decompose::perform(McCAD::Decomposition& inputdata)
    return 0;
}
