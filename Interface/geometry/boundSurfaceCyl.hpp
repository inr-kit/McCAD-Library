#ifndef BOUNDSURFACECYL_HPP
#define BOUNDSURFACECYL_HPP

// C++
// McCAD
#include "boundSurface.hpp"
#include <Standard.hxx>

namespace McCAD::Geometry{
  class BoundSurfaceCyl : public BoundSurface {
  private:
    class Impl;

  public:
    BoundSurfaceCyl();
    ~BoundSurfaceCyl();
    
    Impl* accessBSPImpl() const;

  private:
    std::unique_ptr<Impl> pBSCImpl;
  };
}
#endif //BOUNDSURFACECYL_HPP
