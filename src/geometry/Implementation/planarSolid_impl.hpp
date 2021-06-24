#ifndef PLSOLID_IMPL_HPP
#define PLSOLID_IMPL_HPP

// McCAD
#include "planarSolid.hpp"
#include "solid_impl.hpp"
// OCC
#include <Standard.hxx>

namespace McCAD::Geometry{
  class PLSolid::Impl{
  public:
    void judgeDecomposeSurfaces(Solid::Impl*& solidImpl,
                                Standard_Real precision = 1.0e-7,
                                Standard_Real distanceTolerance = 1.0e-5);
    void judgeThroughConcaveEdges(Solid::Impl*& solidImpl);
  };
}

#endif //PLSOLID_IMPL_HPP
