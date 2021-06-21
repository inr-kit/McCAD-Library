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
// OCC
#include <Standard.hxx>

namespace McCAD::Decomposition{
  class DecomposeSolid::Impl{
  public:
      Impl(const IO::InputConfig& inputConfig);
      Impl(const IO::InputConfig& inputConfig, const Standard_Integer& recurrenceDepth);
      ~Impl();

      IO::InputConfig inputConfig;
      Tools::SolidType solidType;
      Standard_Integer recurrenceDepth;

      Standard_Boolean operator()(std::shared_ptr<Geometry::PLSolid>& solidObj);
      Standard_Boolean operator()(std::shared_ptr<Geometry::CYLSolid>& solidObj);
      Standard_Boolean operator()(std::shared_ptr<Geometry::TORSolid>& solidObj);
      Standard_Boolean operator()(std::shared_ptr<Geometry::MXDSolid>& solidObj);
      static Standard_Boolean throughNoBoundarySurfaces(
              const std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList);
      static Standard_Boolean planeSplitOnlyPlane(
              const std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList);
      Standard_Boolean perform(Geometry::Solid::Impl& solidImpl);
      Standard_Boolean selectSplitSurface(Geometry::Solid::Impl& solidImpl);
      void extractSolids(Geometry::Solid::Impl& solidImpl,
                         const Geometry::Solid::Impl& subSolidImpl,
                         Standard_Integer& i);
  };
}

#endif //DECOMPOSESOLID_IMPL_HPP
