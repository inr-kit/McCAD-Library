#ifndef DECOMPOSITION_IMPL_HPP
#define DECOMPOSITION_IMPL_HPP

// C++
#include <memory>
#include <deque>
#include <variant>
// McCAD
#include "decomposition.hpp"
#include "inputdata_impl.hpp"
#include "planarSolid_impl.hpp"
#include "preprocessor.hpp"
#include "decomposeSolid_impl.hpp"
#include "ShapeView.hpp"
#include <Standard.hxx>
#include "solidType.hpp"
#include "tools_impl.hpp"
// OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Compound.hxx>

namespace McCAD::Decomposition{
  class Decompose::Impl{
  public:
    Impl(const General::InputData& inputData);
    ~Impl();

    Tools::SolidType solidType;

    std::unique_ptr<TopTools_HSequenceOfShape> splitInputSolidsList;
    std::unique_ptr<TopTools_HSequenceOfShape> rejectedInputSolidsList;
    std::unique_ptr<TopTools_HSequenceOfShape> resultSolidsList;
    std::unique_ptr<TopTools_HSequenceOfShape> rejectedsubSolidsList;

    void flattenSolidHierarchy(const Handle_TopTools_HSequenceOfShape& inputSolidsList);
    void perform();
  };
}

#endif //DECOMPOSITION_IMPL_HPP
