#ifndef DECOMPOSITIONIMPL_HPP
#define DECOMPOSITIONIMPL_HPP

// C++
// McCAD
#include "inputdata.hpp"
#include "decomposition.hpp"
// OCC
#include "TCollection_AsciiString.hxx"
#include "TopTools_HSequenceOfShape.hxx"

namespace McCAD::Decomposition{
  class Decompose::Impl {
  public:
    Impl() = default;
    void getInputSolidsList();

  private:

  };
}

#endif //DECOMPOSITIONIMPL_HPP
