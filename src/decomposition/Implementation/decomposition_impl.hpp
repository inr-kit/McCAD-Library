#ifndef DECOMPOSITION_IMPL_HPP
#define DECOMPOSITION_IMPL_HPP

// C++
#include <memory>
#include <deque>
#include <variant>
// McCAD
#include "decomposition.hpp"
#include "inputdata_impl.hpp"
#include "inputconfig.hpp"
#include "planarSolid_impl.hpp"
#include "cylSolid_impl.hpp"
#include "torSolid_impl.hpp"
#include "preprocessor.hpp"
#include "decomposeSolid_impl.hpp"
#include "ShapeView.hpp"
#include "SolidType.hpp"
#include "tools_impl.hpp"
// OCC
#include <Standard.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Compound.hxx>

namespace McCAD::Decomposition{
    class Decompose::Impl{
    public:
        Impl(const General::InputData& inputData, const IO::InputConfig& inputConfig);
        ~Impl();

        Tools::SolidType solidType;
        const IO::InputConfig& inputConfig;
        std::unique_ptr<TopTools_HSequenceOfShape> splitInputSolidsList;
        std::unique_ptr<TopTools_HSequenceOfShape> rejectedInputSolidsList;
        std::unique_ptr<TopTools_HSequenceOfShape> resultSolidsList;
        std::unique_ptr<TopTools_HSequenceOfShape> rejectedsubSolidsList;

        void perform(const TopoDS_Shape& shape);
        void perform();
        void extractSolids(const Geometry::Solid::Impl& solidImpl);
  };
}

#endif //DECOMPOSITION_IMPL_HPP
