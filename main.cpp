#include <iostream>
#include <string>

#include "McCADConfig.hpp"

#include "universal.hpp"
#include "decomposition.hpp"

int main () {
    std::cout << "Testing cmake" << std::endl;
    std::cout << "McCAD version: " << McCAD_VERSION_MAJOR << \
              "." << McCAD_VERSION_MAJOR << std::endl;
    std::cout << "Intinalizing from namespace" << std::endl;
    McCAD::Decomp::Decomposition b;
    McCAD::Decomp::Decomposition c;
    std::string name = "myfile";
    std::cout << "Entered file name is: " << name << std::endl;
    c.run();
    return 0;
}
