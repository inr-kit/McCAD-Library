#ifndef CONVERSION_IMPL_HPP
#define CONVERSION_IMPL_HPP

// C++
#include <memory>
#include <deque>
#include <variant>
// McCAD
#include "conversion.hpp"
#include "inputdata_impl.hpp"
#include "ShapeView.hpp"
#include <Standard.hxx>
// OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Compound.hxx>

namespace McCAD::Conversion {
    class Convert::Impl {
      
    public:
      Impl(const IO::InputConfig& inputConfig,
           const General::InputData& inputData);
      ~Impl();

      std::unique_ptr<TopTools_HSequenceOfShape> splitInputSolidsList;
      std::unique_ptr<TopTools_HSequenceOfShape> rejectedInputSolidsList;

      void flattenSolidHierarchy(const Handle_TopTools_HSequenceOfShape& inputSolidsList);
      void perform(const TopoDS_Shape& shape);
      void perform();
    };
}
#endif //CONVERSION_IMPL_HPP
