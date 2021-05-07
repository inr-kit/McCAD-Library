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
#include "inputconfig.hpp"
#include "SolidType.hpp"
#include "solid_impl.hpp"
// OCC
#include <Standard.hxx>
#include <TopoDS_Shape.hxx>
#include <TCollection_ExtendedString.hxx>

namespace McCAD::Conversion {
    class Convert::Impl{
    public:
      Impl(const IO::InputConfig& inputConfig);
      ~Impl();
    private:
      Tools::SolidType solidType;
      using shape_Name_ID = std::vector<std::tuple<TopoDS_Shape,
                                                   TCollection_ExtendedString,
                                                   Standard_Integer>>;
    public:
      Standard_Boolean rejectCondition = Standard_False;
      shape_Name_ID acceptedInputSolidsList;
      shape_Name_ID rejectedInputSolidsList;
      std::vector<std::shared_ptr<Geometry::Solid>> solidObjList;
      void getGeomData();
    };
}
#endif //CONVERSION_IMPL_HPP
