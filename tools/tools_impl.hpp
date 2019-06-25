#ifndef TOOLSIMPL_HPP
#define TOOLSIMPL_HPP

// C++
// McCAD
#include "tools.hpp"
// OCC
#include "Handle_ShapeFix_FixSmallFace.hxx"
#include "Handle_ShapeFix_Solid.hxx"
#include <ShapeFix_FixSmallFace.hxx>
#include <ShapeFix_Solid.hxx>
#include <TopoDS.hxx>
#include "TopoDS_Solid.hxx"
#include <TopoDS_Shape.hxx>

namespace McCAD::Tools{
  class Preprocessor::Impl {
  public:
    Impl() = default;
    TopoDS_Shape fixedSolidShape;
    TopoDS_Solid fixedSolid;
    TopoDS_Solid finalSolid;
    const TopoDS_Shape& removeSmallFaces(TopoDS_Shape& solidShape);
    const TopoDS_Solid& repairSolid(TopoDS_Solid& solid);
    const TopoDS_Solid& genericFix(TopoDS_Solid& solid);

  private:

  };
}

#endif //TOOLSIMPL_HPP
