#ifndef SURFACE_IMPL_HPP
#define SURFACE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "surface.hpp"
#include "tools_impl.hpp"
// OCC
#include <TopoDS_Face.hxx>

namespace McCAD::Decomposition{
  class Surface::Impl {
  public:
    Impl() = default;

    TopoDS_Face face;
    std::string surfaceType;
    Standard_Integer surfaceNumber;
    Standard_Boolean splitSurface = Standard_False;
    Standard_Integer numberCollidingSurfaces = 0;
    Standard_Integer numberCollidingCurvedSurfaces = 0;
    void initiate(const TopoDS_Face& aFace);

  private:

  };
}

#endif //SURFACE_IMPL_HPP
