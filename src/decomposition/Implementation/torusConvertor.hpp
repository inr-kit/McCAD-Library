#ifndef TORUSCONVERTOR_HPP
#define TORUSCONVERTOR_HPP

// C++
#include <memory>
#include <optional>
#include <vector>
// McCAD
#include "solid_impl.hpp"
// OCC
#include <Standard.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Face.hxx>

namespace McCAD::Decomposition{
  class TorusConvertor{
  public:
    TorusConvertor() = default;

    Standard_Real scalingFactor;
    void operator()(const std::shared_ptr<Geometry::Solid>& solid,
                    const Standard_Real& scalingFactor);
    std::optional<TopoDS_Shape> convertTorusToCylinder(const TopoDS_Shape& shape,
                                                       Standard_Real scaleFactor = 1.6);
    void retrieveSolid(TopoDS_Solid& cylinder,
                       const std::vector<TopoDS_Face>& planesList,
                       Standard_Real innerRadius,
                       const Standard_Real& scaleFactor);
    std::optional<TopoDS_Shape> fitCylinder(TopoDS_Solid& cylinder,
                                              std::vector<TopoDS_Face>& planesList);
    std::optional<std::pair<TopoDS_Shape, TopoDS_Shape>> splitSolid(TopoDS_Solid& solid,
                                                                    TopoDS_Face& splitFace);
  };
}

#endif //TORUSCONVERTOR_HPP
