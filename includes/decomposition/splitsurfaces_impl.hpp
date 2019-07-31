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
    Standard_Boolean planeSplitOnlyPlane(std::vector<std::shared_ptr<McCAD::Decomposition::BoundSurface>>& facesList);
    void generateSplitFacesList(std::vector<std::shared_ptr<McCAD::Decomposition::BoundSurface>>& splitFacesList, std::vector<std::shared_ptr<McCAD::Decomposition::BoundSurface>>& selectedSplitFacesList);
    void sortSplitFaces(std::vector<std::shared_ptr<McCAD::Decomposition::BoundSurface>>& splitFacesList);


  private:

  };
}

#endif //SPLITSURFACES_IMPL_HPP
