#ifndef CONVERSION_IMPL_HPP
#define CONVERSION_IMPL_HPP

// C++
#include <memory>
#include <vector>
#include <tuple>
#include <deque>
#include <variant>
// McCAD
#include "conversion.hpp"
#include "SolidType.hpp"
#include "solid_impl.hpp"
#include "compound.hpp"
// OCC
#include <TopoDS_Shape.hxx>
#include <TCollection_ExtendedString.hxx>

namespace McCAD::Conversion {
    class Convert::Impl{
    public:
        Impl(IO::InputConfig& inputConfig);
        ~Impl();
    private:
        int debugLevel;
        Tools::SolidType solidType;
        using shapeTuple = std::tuple<TopoDS_Shape, TCollection_ExtendedString>;
        using shapesMap = std::vector<shapeTuple>;
        using solidsMap = std::vector<std::tuple<TCollection_ExtendedString,
                                                 TopTools_HSequenceOfShape>>;
    public:
        IO::InputConfig inputConfig;
        shapesMap inputShapesMap;
        std::vector<std::shared_ptr<Geometry::Impl::Compound>> compoundList;
        bool rejectCondition = false;
        solidsMap rejectConversion;
        std::vector<std::shared_ptr<Geometry::Solid>> solidObjList;
        int getGeomData(const std::tuple<std::string, Standard_Real>& matInfo,
                        const int& componentIndex, const int& solidIndex);
    };
}

#endif //CONVERSION_IMPL_HPP
