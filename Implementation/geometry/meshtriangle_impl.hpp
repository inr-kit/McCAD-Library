#ifndef MESHTRIANGLE_IMPL_HPP
#define MESHTRIANGLE_IMPL_HPP

// C++
#include <string>
#include <array>
// McCAD
#include "meshtriangle.hpp"
// OCC
#include <gp_Pnt.hxx>
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <TopoDS_Face.hxx>

namespace McCAD::Geometry{
  class MeshTriangle::Impl {
  public:
    Impl() = default;

    Bnd_Box boundingBox;
    std::array<gp_Pnt, 3> points;

    void initiate(const TopoDS_Face& face);
  };
}

#endif //MESHTRIANGLE_IMPL_HPP
