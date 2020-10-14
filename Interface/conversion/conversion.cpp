// McCAD
#include "conversion.hpp"
#include "conversion_impl.hpp"

McCAD::Conversion::Convert::Convert(const IO::InputConfig& inputConfig,
                                    const General::InputData& inputData) :
  pCImpl{std::make_unique<Impl>(inputConfig, inputData)}{
}

McCAD::Conversion::Convert::~Convert(){
}

McCAD::Conversion::Convert::Impl*
McCAD::Conversion::Convert::accessCImpl() const{
  return pCImpl.get();
}
