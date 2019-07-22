#ifndef MESHTRIANGLE_IMPL_HPP
#define MESHTRIANGLE_IMPL_HPP

// C++
#include <string>
#include <vector>
// McCAD
#include "meshtriangle.hpp"
// OCC
#include <gp_Pnt.hxx>
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <TopoDS_Face.hxx>

namespace McCAD::Decomposition{
  class MeshTriangle::Impl {
  public:
    Impl() = default;

    Bnd_Box boundingBox;
    std::vector<gp_Pnt> points;

    void initiate(const TopoDS_Face& face);

  private:

  };
}

#endif //MESHTRIANGLE_IMPL_HPP
