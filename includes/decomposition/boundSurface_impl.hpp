#ifndef BOUNDSURFACE_IMPL_HPP
#define BOUNDSURFACE_IMPL_HPP

// C++
// McCAD
#include "boundSurface.hpp"
#include "mesh.hpp"
// OCC
#include <Handle_Poly_Triangulation.hxx>
#include <BRepAdaptor_Surface.hxx>

namespace McCAD::Decomposition{
  class BoundSurface::Impl {
  public:
    Impl() = default;

    std::vector<std::unique_ptr<McCAD::Decomposition::Mesh>> meshList;
    
    void initiate();
    Standard_Boolean generateMesh(const Standard_Real& meshDeflection);

  private:

  };
}

#endif //BOUNDSURFACE_IMPL_HPP
