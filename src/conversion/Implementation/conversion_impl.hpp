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
#include <Standard.hxx>
#include <TopoDS_Shape.hxx>
#include <TCollection_ExtendedString.hxx>

namespace McCAD::Conversion {
    class Convert::Impl{
    public:
        Impl(IO::InputConfig& inputConfig);
        ~Impl();
    private:
        Tools::SolidType solidType;
        using shapeTuple = std::tuple<TopoDS_Shape, TCollection_ExtendedString>;
        using shapesMap = std::vector<shapeTuple>;
        using solidsMap = std::vector<std::tuple<TCollection_ExtendedString,
                                                 TopTools_HSequenceOfShape>>;
    public:
        IO::InputConfig inputConfig;
        shapesMap inputShapesMap;
        std::vector<std::shared_ptr<Geometry::Impl::Compound>> compoundList;
        Standard_Boolean rejectCondition = Standard_False;
        solidsMap rejectConversion;
        std::vector<std::shared_ptr<Geometry::Solid>> solidObjList;
        void getGeomData(const std::tuple<std::string, Standard_Real>& matInfo);
    };
}

#endif //CONVERSION_IMPL_HPP
