#ifndef BOUNDSURFACEPLANE_HPP
#define BOUNDSURFACEPLANE_HPP

// C++
#include <memory>
// McCAD
#include "boundSurface.hpp"
#include <Standard.hxx>

namespace McCAD::Geometry{
  class BoundSurfacePlane : public BoundSurface {
  private:
    class Impl;

  public:
    BoundSurfacePlane();
    ~BoundSurfacePlane();
    
    Impl* accessBSPImpl() const;

  private:
    std::unique_ptr<Impl> pBSPImpl;
  };
}
#endif //BOUNDSURFACEPLANE_HPP
