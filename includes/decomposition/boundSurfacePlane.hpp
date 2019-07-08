#ifndef BOUNDSURFACEPLANE_HPP
#define BOUNDSURFACEPLANE_HPP

// C++
#include <memory>
// McCAD
#include "boundSurface.hpp"
#include <Standard.hxx>

namespace McCAD::Decomposition {
  class BoundSurfacePlane : public BoundSurface {
  private:
    class Impl;

  public:
    BoundSurfacePlane();
    ~BoundSurfacePlane();
    
    Impl* accessImpl() const;

  private:
    std::unique_ptr<Impl> pImpl;
  };
}
#endif //BOUNDSURFACEPLANE_HPP
