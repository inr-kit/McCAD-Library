#ifndef DECOMPOSESOLID_IMPL_HPP
#define DECOMPOSESOLID_IMPL_HPP

// C++
#include <variant>
// McCAD
#include "solid_impl.hpp"
#include <Standard.hxx>
#include "ShapeView.hpp"
// OCC
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <GeomAdaptor_Surface.hxx>

namespace McCAD::Decomposition{
  class preprocessor{
  public:
    preprocessor();
    ~preprocessor();

    std::variant<std::monostate, McCAD::Geometry::Solid> perform(const TopoDS_Shape& shape);
    Standard_Boolean checkBndSurfaces(const TopoDS_Solid& solid);
    std::string determineSolidType(const TopoDS_Solid& solid);
    void constructObj(std::variant<std::monostate, McCAD::Geometry::Solid> solidObj,
                      const TopoDS_Solid& solid);
  };
}

#endif //DECOMPOSESOLID_IMPL_HPP
