#ifndef CONSOLID_IMPL_HPP
#define CONSOLID_IMPL_HPP

// McCAD
#include "conSolid.hpp"
#include "solid_impl.hpp"

namespace McCAD::Geometry{
  class CONSolid::Impl {
  public:
    Impl() = default;

    void judgeDecomposeSurfaces(Solid::Impl* solidImpl,
                                Standard_Real precision = 1.0e-6,
                                Standard_Real distanceTolerance = 1.0e-6);
    void judgeAssistDecomposeSurfaces(Solid::Impl* solidImpl,
                                      Standard_Real precision = 1.0e-6,
                                      Standard_Real distanceTolerance = 1.0e-6);
    void judgeThroughConcaveEdges(Solid::Impl* solidImpl);
  };
}

#endif //CONSOLID_IMPL_HPP
