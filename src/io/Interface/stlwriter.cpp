// McCAD
#include "stlwriter.hpp"
#include "stlwriter_impl.hpp"
#include "inputdata_impl.hpp"

McCAD::IO::STLWriter::STLWriter(const std::string& fileName,
                                const General::InputData& inputData) :
    pImpl{std::make_unique<Impl>(fileName)}{
  (*pImpl)(inputData.accessImpl()->inputSolidsList);
}

McCAD::IO::STLWriter::~STLWriter(){

}
