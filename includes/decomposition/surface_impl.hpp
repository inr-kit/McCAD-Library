#ifndef SURFACE_IMPL_HPP
#define SURFACE_IMPL_HPP

// C++
#include <string>
// McCAD
// OCC
#include <Standard.hxx>

namespace McCAD::Decomposition{
  class Surface::Impl {
  public:
    Impl() = default;

    std::string surfaceType;
    Standard_Integer surfaceNumber;
    void initiate();

  private:

  };
}

#endif //SURFACE_IMPL_HPP
