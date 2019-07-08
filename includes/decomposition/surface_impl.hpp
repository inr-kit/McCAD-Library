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
    Impl(McCAD::Decomposition::Surface* surface);
    ~Impl();

    void initiate();

  private:

  };
}

#endif //SURFACE_IMPL_HPP
