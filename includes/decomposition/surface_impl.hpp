#ifndef SURFACE_IMPL_HPP
#define SURFACE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "surface.hpp"
// OCC

namespace McCAD::Decomposition{
  class Surface::Impl {
  public:
    Impl() = default;

    Standard_Integer surfaceNumber;

    virtual std::string getsurfaceType();
    void initiate();

  private:

  };
}

#endif //SURFACE_IMPL_HPP
