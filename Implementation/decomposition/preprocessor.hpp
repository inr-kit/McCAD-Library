#ifndef Preprocessor_HPP
#define Preprocessor_HPP

// C++
#include <variant>
#include <memory>
// McCAD
#include <Standard.hxx>
#include "ShapeView.hpp"
#include "SolidType.hpp"
#include "planarSolid_impl.hpp"
#include "cylSolid_impl.hpp"
#include "createSolidObj.hpp"
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

  private:
    using VariantType = std::variant<std::monostate,
    std::shared_ptr<McCAD::Geometry::PLSolid>,
    std::shared_ptr<McCAD::Geometry::CYLSolid>>;

  public:
    McCAD::Tools::SolidType solidType;

    VariantType perform(const TopoDS_Shape& shape);
    Standard_Boolean checkBndSurfaces(const TopoDS_Solid& solid);
    Standard_Integer determineSolidType(const TopoDS_Solid& solid);
  };
}

#endif //Preprocessor_HPP
