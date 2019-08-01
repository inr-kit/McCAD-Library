#ifndef DECOMPOSITION_IMPL_HPP
#define DECOMPOSITION_IMPL_HPP

// C++
#include <memory>
// McCAD
#include "decomposition.hpp"
#include "inputdata_impl.hpp"
#include "tools_impl.hpp"
#include "decomposeSolid_impl.hpp"
// OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Compound.hxx>

namespace McCAD::Decomposition{
  class Decompose::Impl {
  public:
    Impl(const McCAD::General::InputData& inputData);

    McCAD::Tools::Preprocessor preproc;
    TopoDS_Solid solid;
    TopoDS_Shape solidShape;
    std::unique_ptr<TopTools_HSequenceOfShape> splitInputSolidsList;
    std::unique_ptr<TopTools_HSequenceOfShape> rejectedInputSolidsList;
    std::shared_ptr<TopTools_HSequenceOfShape> resultSolidsList;

    void splitInputSolids(const Handle_TopTools_HSequenceOfShape& inputSolidsList);
    void perform();

  private:

  };
}

#endif //DECOMPOSITION_IMPL_HPP
