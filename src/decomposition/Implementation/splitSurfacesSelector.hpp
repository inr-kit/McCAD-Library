#ifndef SPLITSURFACESSELECTOR_HPP
#define SPLITSURFACESSELECTOR_HPP

// C++
#include <vector>
#include <memory>
// McCAD
#include "boundSurface_impl.hpp"

namespace McCAD::Decomposition{
  class SplitSurfacesSelector{
  public:
    SplitSurfacesSelector();
    ~SplitSurfacesSelector();
    static void generateSplitFacesList(
            std::vector<std::shared_ptr<Geometry::BoundSurface>>& splitFacesList,
            std::vector<std::shared_ptr<Geometry::BoundSurface>>& selectedSplitFacesList);
    static void sortSplitFaces(
            std::vector<std::shared_ptr<Geometry::BoundSurface>>& splitFacesList);
  };
}

#endif //SPLITSURFACESSELECTOR_HPP
