#ifndef CYLSOLID_IMPL_HPP
#define CYLSOLID_IMPL_HPP

// McCAD
#include "cylSolid.hpp"
#include "solid_impl.hpp"
// OCC
#include "Standard.hxx"

namespace McCAD::Geometry{
  class CYLSolid::Impl {
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

#endif //CYLSOLID_IMPL_HPP
