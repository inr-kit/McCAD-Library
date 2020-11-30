#ifndef BOUNDSURFACETOR_HPP
#define BOUNDSURFACETOR_HPP

// C++
// McCAD
#include "boundSurface.hpp"
#include <Standard.hxx>

namespace McCAD::Geometry{
  class BoundSurfaceTor : public BoundSurface {
  private:
    class Impl;

  public:
    BoundSurfaceTor();
    ~BoundSurfaceTor();
    
    Impl* accessBSTImpl() const;

  private:
    std::unique_ptr<Impl> pBSTImpl;
  };
}
#endif //BOUNDSURFACETOR_HPP
