#include <iostream>
#include <string>

#include "McCADConfig.hpp"

#ifdef Materials
#include "materials.hpp"
#endif
#include "solidscollection.hpp"
#include "decomposition.hpp"
#include "materials.hpp"
#include "void.hpp"


int main () {
    std::cout << "Testing cmake" << std::endl;
    std::cout << "McCAD version: " << McCAD_VERSION_MAJOR << \
              "." << McCAD_VERSION_MAJOR << std::endl;
    std::cout << "intinalizing from namespace" << std::endl;
    McCAD::Universal::SolidsCollection a;
    McCAD::Decomp::Decomposition b;
    McCAD::Decomp::Decomposition c;
    c.run();

}
