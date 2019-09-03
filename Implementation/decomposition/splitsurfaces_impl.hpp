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

    Standard_Boolean throughNoBoundarySurfaces(std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList);
    Standard_Boolean planeSplitOnlyPlane(std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList);
    void generateSplitFacesList(std::vector<std::shared_ptr<Geometry::BoundSurface>>& splitFacesList,
				std::vector<std::shared_ptr<Geometry::BoundSurface>>& selectedSplitFacesList);
    void sortSplitFaces(std::vector<std::shared_ptr<Geometry::BoundSurface>>& splitFacesList);

  private:

  };
}

#endif //SPLITSURFACES_IMPL_HPP
