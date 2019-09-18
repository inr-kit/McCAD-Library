#ifndef SURFACE_IMPL_HPP
#define SURFACE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "surface.hpp"
#include "tools_impl.hpp"
// OCC
#include <TopoDS_Face.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <GeomAdaptor_Surface.hxx>

namespace McCAD::Geometry{
  class Surface::Impl {
  public:
    Impl() = default;

    TopoDS_Face face;
    TopoDS_Face extendedFace;
    std::string surfaceType;
    Standard_Integer surfaceNumber;
    Standard_Boolean splitSurface = Standard_False;
    Standard_Integer numberCollidingSurfaces = 0;
    Standard_Integer numberCollidingCurvedSurfaces = 0;
    Standard_Integer throughConcaveEdges = 0;
    
    void initiate(const TopoDS_Face& aFace);
  };
}

#endif //SURFACE_IMPL_HPP
