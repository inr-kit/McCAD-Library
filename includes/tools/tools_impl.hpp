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
#include <BRepGProp.hxx>
#include <ShapeAnalysis_CheckSmallFace.hxx>
#include <TopoDS_Edge.hxx>

namespace McCAD::Tools{
  class Preprocessor::Impl {
  public:
    Impl() = default;

    void removeSmallFaces(TopoDS_Shape& solidShape, Standard_Real precision = 1.0e-3, Standard_Real maxTolerance = 1.0e-3);
    void repairSolid(TopoDS_Solid& solid);
    void genericFix(TopoDS_Solid& solid);
    void fixFace(TopoDS_Face& face, Standard_Real precision = 1.0e-7, Standard_Real maxTolerance = 1.0e-3);
    Standard_Real calcMeshDeflection(const TopoDS_Solid& solid, Standard_Real bndBoxGap = 0.0, Standard_Real converting = 100);
    Standard_Boolean checkBndSurfaces(const TopoDS_Solid& solid);
    Standard_Boolean checkFace(const TopoDS_Face& face, Standard_Real tolerance = 1.0e-3);

  private:

  };
}

#endif //TOOLSIMPL_HPP
