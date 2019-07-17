#ifndef SURFACE_IMPL_HPP
#define SURFACE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "surface.hpp"
// OCC
#include <TopoDS_Face.hxx>

namespace McCAD::Decomposition{
  class Surface::Impl {
  public:
    Impl() = default;

    TopoDS_Face face;
    std::string surfaceType;
    Standard_Integer surfaceNumber;
    
    void initiate(const TopoDS_Face& aFace);

  private:

  };
}

#endif //SURFACE_IMPL_HPP
