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
#include "ShapeFix_FixSmallFace.hxx"
#include "ShapeFix_Solid.hxx"

namespace McCAD::Tools{
  class Preprocessor::Impl {
  public:
    Impl() = default;
    static Handle_TopTools_HSequenceOfShape splitCompSolidsList;
    static TopoDS_Shape fixedSolidShape;
    static TopoDS_Solid fixedSolid;
    void splitCompSolids(Handle_TopTools_HSequenceOfShape& solidsList) const;
    const TopoDS_Shape& removeSmallFaces(TopoDS_Shape& solidShape) const;
    const TopoDS_Solid& repairSolid(TopoDS_Solid& solid) const;

  private:

  };
}

#endif //TOOLSIMPL_HPP
