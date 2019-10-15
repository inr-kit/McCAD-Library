#ifndef Preprocessor_HPP
#define Preprocessor_HPP

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
  class Preprocessor{
  public:
    Preprocessor();
    ~Preprocessor();

    std::unique_ptr<McCAD::Geometry::Solid> perform(const TopoDS_Shape& shape);
    Standard_Boolean checkBndSurfaces(const TopoDS_Solid& solid);
    std::string determineSolidType(const TopoDS_Solid& solid);
    template<typename objType>
    std::unique_ptr<objType> constructObj(const TopoDS_Shape& shape);
  };
}

#endif //Preprocessor_HPP
