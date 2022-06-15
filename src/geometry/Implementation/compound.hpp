#ifndef COMPOUND_IMPL_HPP
#define COMPOUND_IMPL_HPP

// C++
#include <vector>
#include <memory>
#include <tuple>
// McCAD
#include "solid_impl.hpp"
#include "planarSolid_impl.hpp"
#include "cylSolid_impl.hpp"
#include "torSolid_impl.hpp"
#include "mixedSolid_impl.hpp"
// OCCT
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
    int matID{0}, compoundID{0};
    std::tuple<std::string, double> matInfo;

    std::vector<std::shared_ptr<Geometry::Solid>> solidsList;
    std::vector<std::shared_ptr<Geometry::PLSolid>> planarSolidsList;
    std::vector<std::shared_ptr<Geometry::CYLSolid>> cylSolidsList;
    std::vector<std::shared_ptr<Geometry::TORSolid>> torSolidsList;
    std::vector<std::shared_ptr<Geometry::MXDSolid>> mixedSolidsList;
    // Decomposition variables
    std::unique_ptr<TopTools_HSequenceOfShape> acceptedInputShapesList;
    std::unique_ptr<TopTools_HSequenceOfShape> rejectedInputShapesList;
    std::unique_ptr<TopTools_HSequenceOfShape> subSolidsList;
    std::unique_ptr<TopTools_HSequenceOfShape> rejectedsubSolidsList;
    // Conversion variables
    int MCCellID{ 0 };
  };
}

#endif // COMPOUND_IMPL_HPP
