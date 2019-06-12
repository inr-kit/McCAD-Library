#ifndef DECOMPOSITIONIMPL_HPP
#define DECOMPOSITIONIMPL_HPP

// C++
// McCAD
#include "decomposition.hpp"
// OCC
#include "TopTools_HSequenceOfShape.hxx"

namespace McCAD::Decomposition{
  class Decompose::Impl {
  public:
    Impl() = default;

    Handle_TopTools_HSequenceOfShape inputSolidsList;
    Handle_TopTools_HSequenceOfShape splitInputSolidsList;

    void getInputSolidsList();
    void splitInputSolids();

  private:

  };
}

#endif //DECOMPOSITIONIMPL_HPP
