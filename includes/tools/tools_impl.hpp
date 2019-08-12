#ifndef TOOLSIMPL_HPP
#define TOOLSIMPL_HPP

// C++
#include <algorithm>
#include <cmath>
#include <tuple>
#include <array>
// McCAD
#include "tools.hpp"
// OCC
#include <ShapeFix_FixSmallFace.hxx>
#include <ShapeFix_Solid.hxx>
#include <ShapeFix_FixSmallFace.hxx>
#include <ShapeFix_Solid.hxx>
#include <ShapeFix_Shape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <Geom_Surface.hxx>
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
#include <TopAbs_Orientation.hxx>
#include <TopLoc_Location.hxx>
#include <gp_Pln.hxx>
#include <gp_Ax3.hxx>
#include <Geom_Curve.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepTools.hxx>

namespace McCAD::Tools{
  class Preprocessor::Impl {
  public:
    Impl() = default;

    void checkBndSurfaces(const TopoDS_Solid& solid, Standard_Boolean& isTorus, Standard_Boolean& isSpline);
    void removeSmallFaces(TopoDS_Shape& solidShape, Standard_Real precision = 1.0e-3, Standard_Real maxTolerance = 1.0e-3);
    void repairSolid(TopoDS_Solid& solid);
    void fixFace(TopoDS_Face& face, Standard_Real precision = 1.0e-7, Standard_Real maxTolerance = 1.0e-3);
    Standard_Boolean checkFace(const TopoDS_Face& face, Standard_Real tolerance = 1.0e-3);
    gp_Dir normalOnFace(const TopoDS_Face& aFace, const gp_Pnt& aPoint);
    Standard_Boolean isSameEdge(const TopoDS_Edge& firstEdge, const TopoDS_Edge& secondEdge, Standard_Real distanceTolerance = 1.0e-5);
    TopoDS_Face fusePlanes(
            const TopoDS_Face& firstFace,
            const TopoDS_Face& secondFace,
            Standard_Real zeroTolerance = 1.0e-7,
            Standard_Real tolerance = 1.0e-7);

  private:

  };
}

#endif //TOOLSIMPL_HPP
