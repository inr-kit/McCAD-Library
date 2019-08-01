#ifndef INPUTDATA_IMPL_HPP
#define INPUTDATA_IMPL_HPP

// C++
#include <string>
// OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <Handle_Standard_Transient.hxx>
// McCAD
#include "inputdata.hpp"

namespace McCAD::General{
  class InputData::Impl{
  public:
    Impl();
    ~Impl();

    Handle_TopTools_HSequenceOfShape inputSolidsList;
  };
}

#endif //INPUTDATA_IMPL_HPP
