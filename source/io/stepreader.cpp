#include "stepreader.hpp"
#include "stepreader_impl.hpp"
#include "inputdata_impl.hpp"

McCAD::IO::STEPReader::STEPReader(
        const std::string& fileName)
    : pImpl{std::make_unique<Impl>(fileName)}{
  pImpl->readSTEP();
}

McCAD::IO::STEPReader::STEPReader(
        const STEPReader& that)
    : pImpl{new Impl{*that.pImpl}}{
}

McCAD::IO::STEPReader::STEPReader(
        STEPReader&& that)
    : pImpl{std::move(that.pImpl)}{
}

McCAD::IO::STEPReader::~STEPReader(){
}

McCAD::IO::STEPReader&
McCAD::IO::STEPReader::operator=(
        const STEPReader& that){
  this->pImpl.reset(new Impl{*that.pImpl});
  return *this;
}

McCAD::IO::STEPReader&
McCAD::IO::STEPReader::operator=(
        STEPReader&& that){
  this->pImpl = std::move(that.pImpl);
  return *this;
}

McCAD::General::InputData
McCAD::IO::STEPReader::getInputData() const{
  McCAD::General::InputData inputData;
  inputData.accessImpl()->inputSolidsList = pImpl->sequenceOfShape;
  return inputData;
}

const std::string&
McCAD::IO::STEPReader::getFileName() const{
  return pImpl->fileName;
}
