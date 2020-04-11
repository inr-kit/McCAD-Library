//McCAD
#include "edgeOnSurface.hpp"
#include "pointOnSurface.hpp"
#include "boundSurfacePlane_impl.hpp"
#include "tools_impl.hpp"
#include "CurveUtilities.hpp"
//OCC
#include "BRep_Tool.hxx"
#include "gp_Pnt.hxx"
#include <GeomAbs_CurveType.hxx>

Standard_Boolean
McCAD::Decomposition::EdgeOnSurface::operator()(
        const TopoDS_Face& face, const McCAD::Geometry::Edge& aEdge,
        Standard_Real tolerance){
    gp_Pnt startPoint = aEdge.accessEImpl()->startPoint;
    gp_Pnt endPoint   = aEdge.accessEImpl()->endPoint;
    tolerance = BRep_Tool::Tolerance(face);
    if (PointOnSurface{}(face, startPoint, tolerance) &&
            PointOnSurface{}(face, endPoint, tolerance)){
        if (aEdge.accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line)){
            return Standard_True;
        } else{
            gp_Pnt middlePoint = aEdge.accessEImpl()->middlePoint;
            gp_Pnt extraPoint = aEdge.accessEImpl()->extraPoint;
            if (PointOnSurface{}(face, middlePoint, tolerance) &&
                    PointOnSurface{}(face, extraPoint, tolerance)){
                return Standard_True;
            }
        }
    }
    return Standard_False;
}
