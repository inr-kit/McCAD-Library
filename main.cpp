#include <iostream>
#include <stdlib.h>
#include <string>

#include "McCADConfig.hpp"

#ifdef SolidsCollection
#include "solidscollection.hpp"
#endif
#include "decomposition.hpp"
#include "materials.hpp"
#include "void.hpp"


int main () {
    std::cout << "Testing cmake" << std::endl;
    std::cout << "McCAD version: " << McCAD_VERSION_MAJOR << \
              "." << McCAD_VERSION_MAJOR << std::endl;
    std::cout << "intinalizing from namespace" << std::endl;
    #ifdef SolidsCollection
      McCAD::Universal::SolidsCollection a;
    #endif
    McCAD::Decomp::Decomposition b;
    McCAD::Decomp::Decomposition c;
    c.run();

}
