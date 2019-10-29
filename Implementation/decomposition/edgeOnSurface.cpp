//McCAD
#include "edgeOnSurface.hpp"
#include "pointOnSurface.hpp"
#include "boundSurfacePlane_impl.hpp"
//OCC
#include "BRep_Tool.hxx"
#include "gp_Pnt.hxx"

Standard_Boolean
McCAD::Decomposition::EdgeOnSurface::edgeOnPlane(
        const TopoDS_Face& face, const McCAD::Geometry::Edge& aEdge,
        Standard_Real tolerance){
    gp_Pnt startPoint = aEdge.accessEImpl()->startPoint;
    gp_Pnt endPoint   = aEdge.accessEImpl()->endPoint;
    //std::cout << startPoint.X() << "," << startPoint.Y() << "," << startPoint.Z() << "," << std::endl;
    //std::cout << endPoint.X() << "," << endPoint.Y() << "," << endPoint.Z() << "," << std::endl;
    tolerance = BRep_Tool::Tolerance(face);
    if (PointOnSurface{}.pointOnPlane(face, startPoint, tolerance) &&
            PointOnSurface{}.pointOnPlane(face, endPoint, tolerance)){
        //std::cout << "points on surface" << std::endl;
        if (aEdge.accessEImpl()->edgeType == "Line"){
            //std::cout	<< "line" << std::endl;
            return Standard_True;
        } else{
            gp_Pnt middlePoint = aEdge.accessEImpl()->middlePoint;
            gp_Pnt extraPoint = aEdge.accessEImpl()->extraPoint;
            if (PointOnSurface{}.pointOnPlane(face, middlePoint, tolerance) &&
                    PointOnSurface{}.pointOnPlane(face, extraPoint, tolerance)){
                //std::cout << "no line" << std::endl;
                return Standard_True;
            }
        }
    }
    return Standard_False;
}
