//McCAD
#include "pointOnSurface.hpp"
#include "boundSurfacePlane_impl.hpp"
//OCC
#include "TopoDS_Face.hxx"
#include "BRepAdaptor_Surface.hxx"
#include "GeomAdaptor_Surface.hxx"
#include "gp_Pln.hxx"

template<>
Standard_Boolean
McCAD::Decomposition::TriangleCollision::triangleCollision<McCAD::Geometry::BoundSurfacePlane>(
        const TopoDS_Face& face, const McCAD::Geometry::MeshTriangle& aTriangle,
        Standard_Integer& aSide, Standard_Real tolerance,
        Standard_Real tolerance2){
    std::cout << "triangleCollision, Plane" << std::endl;
    Standard_Boolean collision = Standard_False;
    Standard_Integer positivePoints = 0;
    Standard_Integer negativePoints = 0;
    auto& points = aTriangle.accessMTImpl()->points;
    for (Standard_Integer i = 0; i <= points.size() - 1; ++i){
        if (PointOnSurface{}.pointOnSurface<McCAD::Geometry::BoundSurfacePlane>(
                    face, points[i], tolerance)) continue;
        Standard_Real evaluate;
        //BRepAdaptor_Surface surfaceAdaptor(face, Standard_True);
        //GeomAdaptor_Surface surfaceGeomAdaptor= surfaceAdaptor.Surface();
        GeomAdaptor_Surface surfaceGeomAdaptor =
                BRepAdaptor_Surface{face, Standard_True}.Surface();
        gp_Pln Plane = surfaceGeomAdaptor.Plane();
        std::array<Standard_Real, 4> parameters;
        Plane.Coefficients(parameters[0], parameters[1], parameters[2], parameters[3]);
        evaluate = points[i].X()*parameters[0] + points[i].Y()*parameters[1] +
                points[i].Z()*parameters[2] + parameters[3];
      if (evaluate > tolerance2){
          ++positivePoints;
      } else if (evaluate < tolerance2){
          ++negativePoints;
      }
      if (positivePoints > 0 && negativePoints > 0){
          collision = Standard_True;
          break;
      }
    }
    if (positivePoints > 0 && negativePoints == 0){
        aSide = 1;
    } else if (positivePoints == 0 && negativePoints > 0){
        aSide = -1;
    }
    return collision;
}
