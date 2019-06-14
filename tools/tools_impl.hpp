#ifndef TOOLSIMPL_HPP
#define TOOLSIMPL_HPP

// C++
// McCAD
#include "tools.hpp"
// OCC
#include "Handle_TopTools_HSequenceOfShape.hxx"
#include "TopTools_HSequenceOfShape.hxx"
#include "TopExp_Explorer.hxx"
#include "TopoDS.hxx"
#include "TopoDS_Solid.hxx"
#include "TCollection_AsciiString.hxx"
#include "TopoDS_Compound.hxx"

namespace McCAD::Tools{
  class Preprocessor::Impl {
  public:
    Impl() = default;

    const Handle_TopTools_HSequenceOfShape SplitCompSolids(Handle_TopTools_HSequenceOfShape& solidsList) const; 

  private:

  };
}

#endif //TOOLSIMPL_HPP
