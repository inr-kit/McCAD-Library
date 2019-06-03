#ifndef GENERAL_HPP
#define GENERAL_HPP

// C++
// McCAD
#include "generaldetail.hpp"

// Define a universal namespace within McCAD. This namespace should host
// all classes that doesn't belong to the other namespaces (Decomp, Void,
// materials, etc.).
namespace McCAD { namespace General {
    class InputData {
    public:
      InputData() = default;
    private:
      McCAD::General::Detail::InputData_Impl * InputD;
    };
  }}

#endif //GENERAL_HPP
