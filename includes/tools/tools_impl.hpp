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
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>
#include <ElSLib.hxx>
#include <CSLib_DerivativeStatus.hxx>
#include <CSLib.hxx>
#include <Precision.hxx>
#include <BRepAdaptor_Surface.hxx>

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
    std::string getSurfTypeName(const Standard_Integer& index);
    std::string getCurveTypeName(const Standard_Integer& index);
    gp_Dir normalOnFace(const TopoDS_Face& aFace, const gp_Pnt& aPoint);
    Standard_Boolean isSameSurface(const TopoDS_Face& firstFace, const TopoDS_Face& secondFace, Standard_Real distanceTolerance = 1.0e-5, Standard_Real angleTolerance = 1.0e-3, Standard_Real parameterTolerance = 1.0e-7);

  private:

  };
}

#endif //TOOLSIMPL_HPP
