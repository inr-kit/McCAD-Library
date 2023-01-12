//McCAD
#include "edgeOnSurface.hpp"
#include "pointOnSurface.hpp"
#include "boundSurfacePlane_impl.hpp"
#include "tools_impl.hpp"
#include "CurveUtilities.hpp"
//OCCT
#include "BRep_Tool.hxx"
#include "gp_Pnt.hxx"
#include <GeomAbs_CurveType.hxx>

/** ********************************************************************
* @brief    A operator to test if an edge lies on a surface.
* @param    face is a OCCT face.
* @param    aEdge is a McCAD edge object.
* @param    distanceTolerance is used in edges comparison by OCCT and is set in the inputConfig file.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::EdgeOnSurface::operator()(
        const TopoDS_Face& face, const McCAD::Geometry::Edge& aEdge,
        double distanceTolerance){
    gp_Pnt startPoint = aEdge.accessEImpl()->startPoint;
    gp_Pnt endPoint   = aEdge.accessEImpl()->endPoint;
    if (PointOnSurface{}(face, startPoint, distanceTolerance) &&
            PointOnSurface{}(face, endPoint, distanceTolerance)){
        if (aEdge.accessEImpl()->edgeType == Tools::toTypeName(GeomAbs_Line)){
            return true;
        } else{
            gp_Pnt middlePoint = aEdge.accessEImpl()->middlePoint;
            gp_Pnt extraPoint = aEdge.accessEImpl()->extraPoint;
            return (PointOnSurface{}(face, middlePoint, distanceTolerance) &&
                    PointOnSurface{}(face, extraPoint, distanceTolerance));
        }
    }
    return false;
}
