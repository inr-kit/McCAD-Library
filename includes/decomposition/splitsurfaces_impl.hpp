#ifndef SPLITSURFACES_IMPL_HPP
#define SPLITSURFACES_IMPL_HPP

// C++
#include <string>
#include <vector>
#include <memory>
// McCAD
#include "splitsurfaces.hpp"
#include "boundSurface_impl.hpp"
// OCC

namespace McCAD::Decomposition{
  class SplitSurfaces::Impl {
  public:
    Impl() = default;

    Standard_Boolean throughBoundarySurfaces(std::vector<std::shared_ptr<McCAD::Decomposition::BoundSurface>>& facesList);
    Standard_Boolean planeSplitOnPlane(std::vector<std::shared_ptr<McCAD::Decomposition::BoundSurface>>& facesList);


  private:

  };
}

#endif //SPLITSURFACES_IMPL_HPP
