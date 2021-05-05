// McCAD
#include "conversionWriter.hpp"
#include "mcnpWriter.hpp"

McCAD::Conversion::Writer::Writer(const IO::InputConfig& inputConfig) :
    MCOutputFileName{inputConfig.MCOutputFileName}, MCcode{inputConfig.MCcode},
    startCellNum{inputConfig.startCellNum}, startSurfNum{inputConfig.startSurfNum},
    precision{inputConfig.precision} {}

McCAD::Conversion::Writer::~Writer(){}

void
McCAD::Conversion::Writer::operator()(
        const McCAD::Conversion::Writer::solidsList& solidObjList){
    if(MCcode == "mcnp"){
        MCNPWriter{MCOutputFileName, startCellNum, startSurfNum, precision}(solidObjList);
    }
}

void
McCAD::Conversion::Writer::operator()(
        const McCAD::Conversion::Writer::solidsList& solidObjList,
        const std::shared_ptr<VoidCell>& voidCell){
    if(MCcode == "mcnp"){
        MCNPWriter{MCOutputFileName, startCellNum, startSurfNum, precision}(solidObjList, voidCell);
    }
}

