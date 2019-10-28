#ifndef BOUNDSURFACE_IMPL_HPP
#define BOUNDSURFACE_IMPL_HPP

// C++
#include <array>
#include <vector>
#include <cmath>
// McCAD
#include "boundSurface.hpp"
#include "surface_impl.hpp"
#include "meshtriangle_impl.hpp"
#include "edge_impl.hpp"
#include "tools_impl.hpp"
#include "ShapeView.hpp"
#include "CurveUtilities.hpp"
#include "PlaneComparator.hpp"
#include "SurfaceUtilities.hpp"
// OCC
#include <Poly_Triangulation.hxx>
#include <Poly_Triangulation.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS_Face.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TopoDS_Wire.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>                                           
#include <BRepBuilderAPI_MakeFace.hxx>
#include <Extrema_ExtPS.hxx>

namespace McCAD::Geometry{
  class BoundSurface::Impl{
  public:
    Impl(BoundSurface* backReference);
    ~Impl();

    BoundSurface* boundSurface;
    Tools::Preprocessor preproc;
    
    std::vector<std::unique_ptr<Edge>> edgesList;
    std::vector<std::unique_ptr<MeshTriangle>> meshTrianglesList;

    Standard_Boolean isEqual(const BoundSurface& that);
    Standard_Boolean canFuse(const BoundSurface& that);
    Standard_Boolean faceCollision(const BoundSurface& aFace,
                                   Standard_Integer& aSide);
    //Standard_Boolean triangleCollision(const MeshTriangle& aTriangle,
    //                                   Standard_Integer& aSide,
    //                                   Standard_Real tolerance = 1e-2,
    //                                   Standard_Real tolerance2 = 1e-3);
    //Standard_Boolean pointOnSurface(const gp_Pnt& aPoint,
    //                                const Standard_Real& distanceTolerance);
    Standard_Boolean generateMesh(const Standard_Real& meshDeflection);
    void generateEdges(Standard_Real uvTolerance = 1.0e-3);
    Standard_Boolean edgeOnSurface(const Edge& aEdge,
                                   Standard_Real tolerance = 1.0e-4);
    void combineEdges(std::vector<std::unique_ptr<Edge>>& aEdgesList);
  };
}
#endif //BOUNDSURFACE_IMPL_HPP
