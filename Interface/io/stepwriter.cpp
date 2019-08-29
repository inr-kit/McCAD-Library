// McCAD
#include "stepwriter.hpp"
#include "stepwriter_impl.hpp"
#include "inputdata_impl.hpp"

McCAD::IO::STEPWriter::STEPWriter(const std::string& fileName, const General::InputData& inputData) : pImpl{std::make_unique<Impl>(fileName)}{
  (*pImpl)(inputData.accessImpl()->inputSolidsList);
}

McCAD::IO::STEPWriter::STEPWriter(const STEPWriter& that) : pImpl{new Impl{*that.pImpl}}{
}

McCAD::IO::STEPWriter::STEPWriter(STEPWriter&& that) : pImpl{std::move(that.pImpl)}{
}

McCAD::IO::STEPWriter::~STEPWriter(){
}

McCAD::IO::STEPWriter&
McCAD::IO::STEPWriter::operator=(const STEPWriter& that){
  this->pImpl.reset(new Impl{*that.pImpl});
  return *this;
}

McCAD::IO::STEPWriter&
McCAD::IO::STEPWriter::operator=(STEPWriter&& that){
  this->pImpl = std::move(that.pImpl);
  return *this;
}

const std::string&
McCAD::IO::STEPWriter::getFileName() const{
  return pImpl->outputfileName;
}
