#ifndef DECOMPOSESOLID_IMPL_HPP
#define DECOMPOSESOLID_IMPL_HPP

// C++
#include <vector>
#include <memory>
// McCAD
#include "decomposeSolid.hpp"
#include "inputconfig.hpp"
#include "SolidType.hpp"
#include "planarSolid_impl.hpp"
#include "cylSolid_impl.hpp"
#include "torSolid_impl.hpp"
#include "mixedSolid_impl.hpp"
#include "boundSurface_impl.hpp"
#include "solid_impl.hpp"

namespace McCAD::Decomposition{
  class DecomposeSolid::Impl{
  public:
      Impl(const IO::InputConfig& inputConfig);
      Impl(const IO::InputConfig& inputConfig, const int& recurrenceDepth);
      ~Impl();
  private:
      IO::InputConfig inputConfig;
      Tools::SolidType solidType;
      int recurrenceDepth;
  public:
      bool operator()(std::shared_ptr<Geometry::PLSolid>& solidObj);
      bool operator()(std::shared_ptr<Geometry::CYLSolid>& solidObj);
      bool operator()(std::shared_ptr<Geometry::TORSolid>& solidObj);
      bool operator()(std::shared_ptr<Geometry::MXDSolid>& solidObj);
      static bool throughNoBoundarySurfaces(
              const std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList);
      static bool planeSplitOnlyPlane(
              const std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList);
      bool perform(Geometry::Solid::Impl& solidImpl);
      bool selectSplitSurface(Geometry::Solid::Impl& solidImpl);
      void extractSolids(Geometry::Solid::Impl& solidImpl,
                         const Geometry::Solid::Impl& subSolidImpl,
                         int& i);
  };
}

#endif //DECOMPOSESOLID_IMPL_HPP
