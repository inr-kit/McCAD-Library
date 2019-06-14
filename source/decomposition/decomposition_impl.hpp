#ifndef DECOMPOSITIONIMPL_HPP
#define DECOMPOSITIONIMPL_HPP

// C++
// McCAD
#include "decomposition.hpp"
// OCC
#include "Handle_TopTools_HSequenceOfShape.hxx"

namespace McCAD::Decomposition{
  class Decompose::Impl {
  public:
    Impl() = default;

    Handle_TopTools_HSequenceOfShape inputSolidsList;
    Handle_TopTools_HSequenceOfShape splitInputSolidsList;

    void getInputSolidsList();
    void splitInputSolids();
    void decompose();

  private:

  };
}

#endif //DECOMPOSITIONIMPL_HPP
