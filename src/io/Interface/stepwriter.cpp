// McCAD
#include "stepwriter.hpp"
#include "stepwriter_impl.hpp"
#include "inputdata_impl.hpp"

McCAD::IO::STEPWriter::STEPWriter(const IO::InputConfig& inputConfig,
                                  const General::InputData& outputData)
    : pImpl{std::make_unique<Impl>(inputConfig)}{
  (*pImpl)(outputData.accessImpl()->outputShapesMap);
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
