// McCAD
#include "conversionWriter.hpp"
#include "mcnpWriter.hpp"

McCAD::Conversion::Writer::Writer(const IO::InputConfig& inputConfig) :
    inputConfig{inputConfig}, MCcode{inputConfig.MCcode} {}

McCAD::Conversion::Writer::~Writer(){}

void
McCAD::Conversion::Writer::operator()(
        const std::vector<std::shared_ptr<Geometry::Impl::Compound>> compoundList,
        const std::shared_ptr<VoidCell>& voidCell){
    if(MCcode == "mcnp"){
        MCNPWriter{inputConfig}(compoundList, voidCell);
    }
}

