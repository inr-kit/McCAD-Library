#ifndef DECOMPOSITION_IMPL_HPP
#define DECOMPOSITION_IMPL_HPP

// C++
#include <memory>
#include <deque>
#include <variant>
#include <vector>
#include <tuple>
// McCAD
#include "decomposition.hpp"
#include "inputdata_impl.hpp"
#include "inputconfig.hpp"
#include "compound.hpp"
#include "solid_impl.hpp"
#include "SolidType.hpp"
#include "tools_impl.hpp"
// OCCT
#include <TopoDS_Shape.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TCollection_ExtendedString.hxx>

namespace McCAD::Decomposition{
    class Decompose::Impl{
    public:
        Impl(const General::InputData& inputData, const IO::InputConfig& inputConfig);
        ~Impl();
    private:
        Tools::SolidType solidType;
        using shapeTuple = std::tuple<TopoDS_Shape, TCollection_ExtendedString>;
        using shapesMap = std::vector<shapeTuple>;
        using solidsMap = std::vector<std::tuple<TCollection_ExtendedString,
                                                 TopTools_HSequenceOfShape>>;
    public:
        shapesMap inputShapesMap;
        IO::InputConfig inputConfig;
        std::vector<std::shared_ptr<Geometry::Impl::Compound>> compoundList;
        solidsMap successDecomposition, rejectDecomposition;
        void perform();
        void perform(const shapeTuple& inputShape, const int& compoundID);
        void extractSolids(
                const std::shared_ptr<Geometry::Impl::Compound>& compound,
                const std::shared_ptr<Geometry::Solid>& solid);
  };
}

#endif //DECOMPOSITION_IMPL_HPP
