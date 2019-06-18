// McCAD
#include "inputdata_impl.hpp"

McCAD::General::InputData::Impl::Impl(
  const std::string& fileName)
  : fileName{fileName}{
}

void McCAD::General::InputData::Impl::setFileName(
  const std::string& fileName){
  this->fileName = fileName;
}

const std::string& McCAD::General::InputData::Impl::getFileName(){
  return fileName;
}

void McCAD::General::InputData::Impl::getInputSolidsList(){
  // Populate the inpuSolidsList
  // this->inputSolidsList = list of solids from the STEP file
}
