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
                                        Standard_Real scaleFactor = 1.1);
    std::optional<TopoDS_Shape> retrieveSolid(TopoDS_Solid& cylinder,
                                              std::vector<TopoDS_Face>& planesList);
  };
}

#endif //TORUSCONVERTOR_HPP
