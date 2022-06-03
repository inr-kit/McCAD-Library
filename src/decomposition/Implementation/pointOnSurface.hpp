#ifndef POINTONSURFACE_HPP
#define POINTONSURFACE_HPP

//OCC
#include <Standard.hxx>
#include <TopoDS_Face.hxx>
#include "gp_Pnt.hxx"

namespace McCAD::Decomposition{
    class PointOnSurface{
    public:
        PointOnSurface() = default;

        Standard_Boolean operator()(const TopoDS_Face& face,
                                    const gp_Pnt& aPoint,
                                    const Standard_Real& distanceTolerance);
    };
}

#endif //POINTONSURFACE_HPP