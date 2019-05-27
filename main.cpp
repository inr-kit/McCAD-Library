#include <iostream>
#include <string>

#include "McCADConfig.hpp"

#ifdef Materials
  #include "materials.hpp"
#endif
#include "universal.hpp"
#include "decomposition.hpp"
#include "inputdata.hpp"
#include "void.hpp"

int main () {
    std::cout << "Testing cmake" << std::endl;
    std::cout << "McCAD version: " << McCAD_VERSION_MAJOR << \
              "." << McCAD_VERSION_MAJOR << std::endl;
    std::cout << "Intinalizing from namespace" << std::endl;
    McCAD::Interface::InputData a;
    McCAD::Decomp::Decomposition b;
    McCAD::Decomp::Decomposition c;
    std::string name = "myfile";
    std::cout << "Entered file name is: " << name << std::endl;
    std::cout << "File name is: " << a.SetFileName(name) << std::endl;
    c.run();
    return 0;
}
