#ifndef CONVERSION_IMPL_HPP
#define CONVERSION_IMPL_HPP

// C++
#include <memory>
#include <deque>
#include <variant>
// McCAD
#include "inputconfig.hpp"
#include "conversion.hpp"
// OCC
#include <Standard.hxx>
#include <TopTools_HSequenceOfShape.hxx>

namespace McCAD::Conversion {
    class Convert::Impl{
    public:
      Impl(const IO::InputConfig& inputConfig);
      ~Impl();

      std::shared_ptr<TopTools_HSequenceOfShape> splitInputSolidsList;
      std::shared_ptr<TopTools_HSequenceOfShape> rejectedInputSolidsList;

      void getGeomData();
      void getMatData();
      void perform();
    };
}
#endif //CONVERSION_IMPL_HPP
