#ifndef BOUNDSURFACEPLANE_IMPL_HPP
#define BOUNDSURFACEPLANE_IMPL_HPP

// C++
#include <string>
#include <memory>
// McCAD
#include "boundSurfacePlane.hpp"
#include "surface_impl.hpp"
// OCC
#include <TopLoc_Location.hxx>
#include <Handle_Geom_Surface.hxx>
#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

namespace McCAD::Decomposition{
  class BoundSurfacePlane::Impl {
  public:
    Impl(McCAD::Decomposition::BoundSurfacePlane* backReference);
    ~Impl();

    McCAD::Decomposition::BoundSurfacePlane* boundSurfacePlane;

    std::string surfaceType = "Plane";
    void generateExtendedPlane(const Standard_Real& boxSquareLength, Standard_Real tolerance = 1.0e-7);
    
  private:

  };
}

#endif //BOUNDSURFACEPLANE_IMPL_HPP
