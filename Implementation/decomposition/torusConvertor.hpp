#ifndef TORUSCONVERTOR_HPP
#define TORUSCONVERTOR_HPP

// C++
#include <memory>
// McCAD
#include <Standard.hxx>
#include "solid_impl.hpp"
// OCC
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepAdaptor_Surface.hxx>

namespace McCAD::Decomposition{
  class TorusConvertor{
  public:
    TorusConvertor() = default;

    void operator()(Geometry::Solid::Impl& solidImpl);
    TopoDS_Shape convertTorusToCylinder(const TopoDS_Shape& shape,
                                        Standard_Real scaleFactor = 2.0);
    void createSolid(TopoDS_Face& cylinder,
                             std::vector<BRepAdaptor_Surface>& planesList);
  };
}

#endif //TORUSCONVERTOR_HPP
