#ifndef POINTONSURFACE_HPP
#define POINTONSURFACE_HPP

//C++
//McCAD
#include <Standard.hxx>
//OCC
#include <TopoDS_Face.hxx>
#include "gp_Pnt.hxx"

namespace McCAD::Decomposition{
    class PointOnSurface{
    public:
        PointOnSurface() = default;

        Standard_Boolean pointOnPlane(const TopoDS_Face& face,
                                      const gp_Pnt& aPoint,
                                      const Standard_Real& distanceTolerance);
    };
}

#endif //POINTONSURFACE_HPP
