#ifndef CONVERSION_IMPL_HPP
#define CONVERSION_IMPL_HPP

// C++
#include <memory>
// McCAD
#include "conversion.hpp"
#include "inputdata.hpp"

namespace McCAD::Conversion {
    class Convert::Impl {
      
    public:
      Impl(const General::InputData& inputData);
      ~Impl();
    };
}
#endif //CONVERSION_IMPL_HPP
