#ifndef BOUNDSURFACE_IMPL_HPP
#define BOUNDSURFACE_IMPL_HPP

// C++
#include <vector>
#include <cmath>
// McCAD
#include "boundSurface.hpp"
#include "surface_impl.hpp"
#include "meshtriangle_impl.hpp"
#include "edge_impl.hpp"
#include "tools_impl.hpp"
// OCC
#include <Handle_Poly_Triangulation.hxx>
#include <Poly_Triangulation.hxx>
#include <Handle_Geom_Surface.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS_Face.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TopoDS_Wire.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>                                           
#include <BRepBuilderAPI_MakeFace.hxx>
#include <Extrema_ExtPS.hxx>
//#include <gp_Pln.hxx>
//#include <gp_Ax3.hxx>
//#include <TopExp_Explorer.hxx>
//#include <TopAbs_ShapeEnum.hxx>
//#include <TopoDS.hxx>
//#include <TopTools_HSequenceOfShape.hxx>
//#include <TCollection_AsciiString.hxx>
//#include <BRepTools.hxx>
//#include <BRepBndLib.hxx>
//#include <Bnd_Box.hxx>
//#include <BRepAlgoAPI_Section.hxx>
//#include <Handle_TColgp_HSequenceOfPnt.hxx>
//#include <GeomAdaptor_Curve.hxx>

namespace McCAD::Decomposition{
  class BoundSurface::Impl {
  public:
    Impl(McCAD::Decomposition::BoundSurface* backReference);
    ~Impl();

    McCAD::Decomposition::BoundSurface* boundSurface;
    McCAD::Tools::Preprocessor preproc;
    std::vector<std::unique_ptr<McCAD::Decomposition::Edge>> edgesList;
    std::vector<std::unique_ptr<McCAD::Decomposition::MeshTriangle>> meshTrianglesList;
    Standard_Boolean isEqual(const McCAD::Decomposition::BoundSurface& that);
    Standard_Boolean canFuse(const McCAD::Decomposition::BoundSurface& that);
    Standard_Boolean faceCollision(const McCAD::Decomposition::BoundSurface& aFace, Standard_Integer& aSide);
    Standard_Boolean triangleCollision(const McCAD::Decomposition::MeshTriangle& aTriangle, Standard_Integer& aSide, Standard_Real tolerance = 1e-2, Standard_Real tolerance2 = 1e-3);
    Standard_Boolean pointOnSurface(const gp_Pnt& aPoint, const Standard_Real& distanceTolerance);
    Standard_Boolean generateMesh(const Standard_Real& meshDeflection);

  private:

  };
}

#endif //BOUNDSURFACE_IMPL_HPP
