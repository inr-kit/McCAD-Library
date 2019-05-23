#include <iostream>
#include <stdlib.h>
#include <string>

#include "McCADConfig.hpp"

#ifdef USE_SolidsCollection
#include "solidscollection.hpp"
#endif

#ifdef USE_Decomposition
#include "decomposition.hpp"
#endif

int main () {
    std::cout << "Testing cmake" << std::endl;
    std::cout << "McCAD version: " << McCAD_VERSION_MAJOR << \
              "." << McCAD_VERSION_MAJOR << std::endl;
    std::cout << "intinalizing from namespace" << std::endl;
    #ifdef USE_SolidsCollection
      McCAD::Universal::SolidsCollection a;
    #endif
    #ifdef USE_Decomposition
      McCAD::Decomp::Decomposition b;
    #endif

}
