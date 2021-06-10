// McCAD
#include "conversion.hpp"
#include "conversion_impl.hpp"

McCAD::Conversion::Convert::Convert(IO::InputConfig& inputConfig) :
  pCImpl{std::make_unique<Impl>(inputConfig)}{
}

McCAD::Conversion::Convert::~Convert(){
}

McCAD::Conversion::Convert::Impl*
McCAD::Conversion::Convert::accessCImpl() const{
  return pCImpl.get();
}
