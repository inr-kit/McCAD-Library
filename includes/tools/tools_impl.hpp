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
#include <ShapeFix_Shape.hxx>
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
#include <BRepGProp.hxx>
#include <ShapeAnalysis_CheckSmallFace.hxx>
#include <TopoDS_Edge.hxx>

namespace McCAD::Tools{
  class Preprocessor::Impl {
  public:
    Impl() = default;

    void removeSmallFaces(TopoDS_Shape& solidShape, const Standard_Real precision = 1.0e-3, const Standard_Real maxTolerance = 1.0e-3);
    void repairSolid(TopoDS_Solid& solid);
    void genericFix(TopoDS_Solid& solid);
    void fixFace(TopoDS_Face& face, const Standard_Real precision = 1.0e-7, const Standard_Real maxTolerance = 1.0e-3);
    const Standard_Real calcMeshDeflection(TopoDS_Solid& solid, const Standard_Real bndBoxGap = 0.0, const Standard_Real converting = 100);
    const Standard_Boolean checkBndSurfaces(TopoDS_Solid& solid);
    const Standard_Boolean checkFace(const TopoDS_Face& face, const Standard_Real tolerance = 1.0e-3);

  private:

  };
}

#endif //TOOLSIMPL_HPP
