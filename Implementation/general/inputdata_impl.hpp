#ifndef INPUTDATA_IMPL_HPP
#define INPUTDATA_IMPL_HPP

// C++
#include <string>
// McCAD
#include "inputdata.hpp"
// OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <Standard_Transient.hxx>

namespace McCAD::General{
  class InputData::Impl{
  public:
    Impl();
    ~Impl();

    Handle_TopTools_HSequenceOfShape inputSolidsList;
  };
}

#endif //INPUTDATA_IMPL_HPP
