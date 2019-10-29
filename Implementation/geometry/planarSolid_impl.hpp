#ifndef PLSOLID_IMPL_HPP
#define PLSOLID_IMPL_HPP

// C++
#include<memory>
// McCAD
#include "planarSolid.hpp"
#include "solid_impl.hpp"
// OCC

namespace McCAD::Geometry{
  class PLSolid::Impl{
  public:
    Impl();
    ~Impl();

    void judgeDecomposeSurfaces(Solid::Impl*& solidImpl);
    void judgeThroughConcaveEdges(Solid::Impl*& solidImpl);
  };
}

#endif //PLSOLID_IMPL_HPP
