#ifndef BOUNDSURFACEPLANE_IMPL_HPP
#define BOUNDSURFACEPLANE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "boundSurfacePlane.hpp"
// OCC
#include <Handle_TopTools_HSequenceOfShape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Compound.hxx>

namespace McCAD::Decomposition{
  class BoundSurfacePlane::Impl {
  public:
    Impl() = default;

    std::string surfaceType = "Plane";
    void initiate(const TopoDS_Face& face);
    void generateExtPlane(const Standard_Real& boxSquareLength);
    
  private:

  };
}

#endif //BOUNDSURFACEPLANE_IMPL_HPP
