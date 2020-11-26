#ifndef EXTENDEDFACEGENERATOR_HPP
#define EXTENDEDFACEGENERATOR_HPP

// C++
// McCAD
#include "boundSurface_impl.hpp"
#include "boundSurfacePlane_impl.hpp"
#include "boundSurfaceCyl_impl.hpp"
#include "boundSurfaceTor_impl.hpp"
// OCC

namespace McCAD::Decomposition{
  class ExtendedFaceGenerator{
  public:
    ExtendedFaceGenerator() = default;

    template <typename >
    TopoDS_Face operator()(Geometry::);
  };
}

#endif //EXTENDEDFACEGENERATOR_HPP
