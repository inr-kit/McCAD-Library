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
    std::optional<TopoDS_Shape> convertTorusToCylinder(const TopoDS_Shape& shape,
                                                       Standard_Real scaleFactor = 1.5);
    void retrieveSolid(TopoDS_Solid& cylinder,
                       const std::vector<TopoDS_Face>& planesList,
                       Standard_Real torusRadius,
                       const Standard_Real& scaleFactor);
    std::optional<TopoDS_Shape> fitCylinder(TopoDS_Solid& cylinder,
                                              std::vector<TopoDS_Face>& planesList);
    std::optional<std::pair<TopoDS_Shape, TopoDS_Shape>> splitSolid(TopoDS_Solid& solid,
                                                                    TopoDS_Face& splitFace);
  };
}

#endif //TORUSCONVERTOR_HPP
