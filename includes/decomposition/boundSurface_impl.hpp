#ifndef BOUNDSURFACE_IMPL_HPP
#define BOUNDSURFACE_IMPL_HPP

// C++
// McCAD
#include "boundSurface.hpp"
// OCC
#include <Handle_TopTools_HSequenceOfShape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Compound.hxx>

namespace McCAD::Decomposition{
  class BoundSurface::Impl {
  public:
    Impl(McCAD::Decomposition::BoundSurface* bndSurface);
    ~Impl();

    McCAD::Decomposition::BoundSurface* bndSurface;
    void initiate();
    Standard_Boolean generateMesh(const Standard_Real& meshDeflection);

  private:

  };
}

#endif //BOUNDSURFACE_IMPL_HPP
