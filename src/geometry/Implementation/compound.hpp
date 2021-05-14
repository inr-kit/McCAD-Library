#ifndef COMPOUND_IMPL_HPP
#define COMPOUND_IMPL_HPP

// C++
#include <vector>
#include <memory>
// McCAD
#include <solid_impl.hpp>
#include "planarSolid_impl.hpp"
#include "cylSolid_impl.hpp"
#include "torSolid_impl.hpp"
// OCC
#include <Standard.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TCollection_ExtendedString.hxx>

namespace McCAD::Geometry::Impl{
  class Compound {
  public:
    Compound(const TopoDS_Shape& shape, const TCollection_ExtendedString& name);
    ~Compound();

    TopoDS_Shape compoundShape;
    TCollection_ExtendedString compoundName;
    Standard_Integer matID{0};
    Standard_Real matDensity{0};

    std::vector<std::shared_ptr<Geometry::Solid>> solidsList;
    std::vector<std::shared_ptr<Geometry::PLSolid>> planarSolidsList;
    std::vector<std::shared_ptr<Geometry::CYLSolid>> cylSolidsList;
    std::vector<std::shared_ptr<Geometry::TORSolid>> torSolidsList;

    std::unique_ptr<TopTools_HSequenceOfShape> acceptedInputShapesList;
    std::unique_ptr<TopTools_HSequenceOfShape> rejectedInputShapesList;
    std::unique_ptr<TopTools_HSequenceOfShape> subSolidsList;
    std::unique_ptr<TopTools_HSequenceOfShape> rejectedsubSolidsList;
  };
}

#endif // COMPOUND_IMPL_HPP
