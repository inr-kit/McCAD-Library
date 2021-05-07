// McCAD
#include "conversionWriter.hpp"
#include "mcnpWriter.hpp"

McCAD::Conversion::Writer::Writer(const IO::InputConfig& inputConfig) :
    inputConfig{inputConfig}, MCcode{inputConfig.MCcode} {}

McCAD::Conversion::Writer::~Writer(){}

void
McCAD::Conversion::Writer::operator()(
        const McCAD::Conversion::Writer::solidsList& solidObjList,
        const std::shared_ptr<VoidCell>& voidCell){
    if(MCcode == "mcnp"){
        MCNPWriter{inputConfig}(solidObjList, voidCell);
    }
}

