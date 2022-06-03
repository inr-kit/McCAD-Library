#ifndef MESHTRIANGLE_IMPL_HPP
#define MESHTRIANGLE_IMPL_HPP

// C++
#include <array>
// McCAD
#include "meshtriangle.hpp"
// OCC
#include <Standard.hxx>
#include <gp_Pnt.hxx>
#include <Bnd_OBB.hxx>
#include <TopoDS_Face.hxx>

namespace McCAD::Geometry{
  class MeshTriangle::Impl {
  public:
    Impl() = default;

    Bnd_OBB obb;
    TopoDS_Face face;
    std::array<gp_Pnt, 3> points;

    void initiate(const TopoDS_Face& aFace);
    void createOBB(const TopoDS_Face& aFace, Standard_Real bndBoxGap = 0.0);
  };
}

#endif //MESHTRIANGLE_IMPL_HPP
