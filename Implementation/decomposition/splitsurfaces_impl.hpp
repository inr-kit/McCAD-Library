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

    // Should probably not be part of this class.
    // No interaction of this function with other members.
    static bool throughNoBoundarySurfaces(
            const std::vector<std::shared_ptr<BoundSurface>>& facesList);

    // Should probably not be part of this class.
    // No interaction of this function with other members.
    static Standard_Boolean planeSplitOnlyPlane(
            std::vector<std::shared_ptr<BoundSurface>>& facesList);

    // Possibly, these should not be part of this class
    static void generateSplitFacesList(
            std::vector<std::shared_ptr<BoundSurface>>& splitFacesList,
            std::vector<std::shared_ptr<BoundSurface>>& selectedSplitFacesList);
    static void sortSplitFaces(
            std::vector<std::shared_ptr<BoundSurface>>& splitFacesList);

  private:

  };
}

#endif //SPLITSURFACES_IMPL_HPP
