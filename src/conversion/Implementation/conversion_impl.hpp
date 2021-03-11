#ifndef CONVERSION_IMPL_HPP
#define CONVERSION_IMPL_HPP

// C++
#include <memory>
#include <deque>
#include <variant>
// McCAD
#include "conversion.hpp"
#include "inputconfig.hpp"
#include "SolidType.hpp"
#include "solid_impl.hpp"
// OCC
#include <Standard.hxx>
#include <TopTools_HSequenceOfShape.hxx>

namespace McCAD::Conversion {
    class Convert::Impl{
    public:
      Impl(const IO::InputConfig& inputConfig);
      ~Impl();

      Tools::SolidType solidType;
      std::shared_ptr<TopTools_HSequenceOfShape> splitInputSolidsList;
      std::shared_ptr<TopTools_HSequenceOfShape> rejectedInputSolidsList;
      std::vector<std::shared_ptr<Geometry::Solid>> solidsList;

      void getGeomData();
      void getMatData();
      void perform();
    };
}
#endif //CONVERSION_IMPL_HPP
