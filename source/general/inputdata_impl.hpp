#ifndef INPUTDATAIMPL_HPP
#define INPUTDATAIMPL_HPP

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
    Impl() = default;

    Handle_TopTools_HSequenceOfShape inputSolidsList;
  };
}

#endif //INPUTDATAIMPL_HPP
