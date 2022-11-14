#ifndef POINTONSURFACE_HPP
#define POINTONSURFACE_HPP

//OCCT
#include <TopoDS_Face.hxx>
#include "gp_Pnt.hxx"

namespace McCAD::Decomposition{
    class PointOnSurface{
    public:
        PointOnSurface() = default;

        bool operator()(const TopoDS_Face& face,
                                    const gp_Pnt& aPoint,
                                    const double& distanceTolerance);
    };
}

#endif //POINTONSURFACE_HPP
