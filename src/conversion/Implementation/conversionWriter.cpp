// McCAD
#include "conversionWriter.hpp"
#include "mcnpWriter.hpp"
#include "mcxWriter.hpp"

McCAD::Conversion::Writer::Writer(const IO::InputConfig& inputConfig) :
    inputConfig{inputConfig}{}

McCAD::Conversion::Writer::~Writer(){}

/** ********************************************************************
* @brief   The operator calls a specialized writer per user parameters.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::Writer::operator()(
        const std::vector<std::shared_ptr<Geometry::Impl::Compound>> compoundList,
        const std::shared_ptr<VoidCell>& voidCell){
    //std::cout << "judge which type of mc cards\n";
    if(inputConfig.MCcode == "mcnp"){
        //std::cout << "mcnp input\n";
        MCNPWriter{inputConfig}(compoundList, voidCell);
    }else if(inputConfig.MCcode == "mcx") {
        //std::cout << "necp-mcx input\n";
        MCXWriter{inputConfig }(compoundList, voidCell);
    }
}

