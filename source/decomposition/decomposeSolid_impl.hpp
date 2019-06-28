#ifndef DECOMPOSESOLID_IMPL_HPP
#define DECOMPOSESOLID_IMPL_HPP

// C++
// McCAD
#include "decomposeSolid.hpp"
// OCC
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>

namespace McCAD::Decomposition{
  class DecomposeSolid::Impl {
  public:
    Impl() = default;

    Standard_Integer recursivityLevel = 0;
    void perform(const TopoDS_Solid& solid, const Standard_Integer& meshDeflection);
    void genSurfaceList(const TopoDS_Solid& solid);

  private:

  };
}

#endif //DECOMPOSESOLID_IMPL_HPP
