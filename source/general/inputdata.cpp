// McCAD
#include "inputdata.hpp"
#include "inputdata_impl.hpp"

McCAD::General::InputData::InputData()
  : pImpl{std::make_unique<Impl>()}{
}

McCAD::General::InputData::InputData(const std::string& fileName)
  : pImpl{std::make_unique<Impl>(fileName)}{
}

McCAD::General::InputData::~InputData(){
}

const McCAD::General::InputData::Impl*
McCAD::General::InputData::accessImpl() const{
  return pImpl.get();
}

void McCAD::General::InputData::setFileName(const std::string& fileName){
  pImpl->setFileName(fileName);
}

const std::string& McCAD::General::InputData::getFileName() const{
  return pImpl->getFileName();
}

void McCAD::General::InputData::getInputSolidsList() const{
  pImpl->getInputSolidsList();
}
