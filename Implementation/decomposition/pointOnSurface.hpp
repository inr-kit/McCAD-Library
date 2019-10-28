#ifndef POINTONSURFACE_HPP
#define POINTONSURFACE_HPP

//C++
#include <memory>
//McCAD
#include "boundSurface_impl.hpp"
//OCC
#include <TopoDS_Face.hxx>
#include <Standard.hxx>

namespace McCAD::Decomposition{
    class PointOnSurface{
    public:
      PointOnSurface();
      ~PointOnSurface();

      template<typename surfaceType>
      Standard_Boolean pointOnSurface(const gp_Pnt& aPoint,
                                      const TopoDS_Face& extendedFace,
                                      const Standard_Real& distanceTolerance);
    };
}

#include "pointOnSurface.tpp"

#endif //POINTONSURFACE_HPP
