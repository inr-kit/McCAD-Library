// C++
#include <iostream>
#include <string>
// McCAD
#include "McCADConfig.hpp"
#include "general.hpp"
#include "decomposition.hpp"

int main () {

    std::string fileName = "SomeFileName";
    McCAD::General::InputData inputData;
    inputData.setFileName(fileName);
    std::cout << "Loading file: "
            << inputData.getFileName()
            << std::endl;
    return 0;

}
