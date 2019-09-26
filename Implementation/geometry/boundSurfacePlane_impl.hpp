#ifndef BOUNDSURFACEPLANE_IMPL_HPP
#define BOUNDSURFACEPLANE_IMPL_HPP

// C++
#include <string>
#include <memory>
#include <array>
#include <vector>
// McCAD
#include "boundSurfacePlane.hpp"
#include "surface_impl.hpp"
// OCC
#include <TopLoc_Location.hxx>
#include <Geom_Surface.hxx>
#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

namespace McCAD::Geometry{
  class BoundSurfacePlane::Impl {
  public:
    Impl(BoundSurfacePlane* backReference);
    ~Impl();

    BoundSurfacePlane* boundSurfacePlane;

    std::string surfaceType = "Plane";
    void generateExtendedPlane(const Standard_Real& boxSquareLength,
			       Standard_Real degenerateEdgesTolerance = 1.0e-7);
  };
}

#endif //BOUNDSURFACEPLANE_IMPL_HPP
