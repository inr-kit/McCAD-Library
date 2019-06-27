#ifndef TOOLSIMPL_HPP
#define TOOLSIMPL_HPP

// C++
#include <algorithm>
// McCAD
#include "tools.hpp"
// OCC
#include <Handle_ShapeFix_FixSmallFace.hxx>
#include <Handle_ShapeFix_Solid.hxx>
#include <ShapeFix_FixSmallFace.hxx>
#include <ShapeFix_Solid.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <Handle_Geom_Surface.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <BRep_Tool.hxx>
#include <Standard.hxx>

namespace McCAD::Tools{
  class Preprocessor::Impl {
  public:
    Impl() = default;

    const TopoDS_Shape removeSmallFaces(TopoDS_Shape& solidShape);
    const TopoDS_Solid repairSolid(TopoDS_Solid& solid);
    const TopoDS_Solid genericFix(TopoDS_Solid& solid);
    const Standard_Real calMeshDeflection(TopoDS_Solid& solid);
    const Standard_Boolean checkBndSurfaces(TopoDS_Solid& solid);

  private:

  };
}

#endif //TOOLSIMPL_HPP
