// McCAD
#include "manager.hpp"
#include "stepreader.hpp"
#include "stepwriter.hpp"
#include "inputdata.hpp"
#include "decomposition.hpp"

McCAD::IO::Manager::Manager(McCAD::IO::InputConfig& inputConfig){
    if (inputConfig.decompose == True){
        auto decomposeObj = decompose(inputConfig);
        if (inputConfig.convert == True && decomposeObj.)
    } else if (inputConfig.convert == True){
        convert(inputConfig);}
}

McCAD::Decomposition::Decompose
McCAD::IO::Manager::decompose(McCAD::IO::InputConfig& inpufConfig){
    // Load the input file.
    std::cout << "***********************" << std::endl;
    std::cout << "** Loading STEP file **" << std::endl;
    std::cout << "***********************" << std::endl;
    McCAD::IO::STEPReader reader{inpufConfig.inputFileName};
    auto inputData = reader.getInputData();
    // Start decomposition.
    std::cout << "****************************" << std::endl;
    std::cout << "** Starting decomposition **" << std::endl;
    std::cout << "****************************" << std::endl;
    McCAD::Decomposition::Decompose decompose{inputData};
    auto outputData_result = decompose.getResultSolids();
    auto outputData_reject = decompose.getRejectedSolids();
    // Write output STEP files.
    std::cout << "*************************" << std::endl;
    std::cout << "** Saving to STEP file **" << std::endl;
    std::cout << "*************************" << std::endl;
    McCAD::IO::STEPWriter{inpufConfig.resultFileName, outputData_result};
    McCAD::IO::STEPWriter{inpufConfig.rejectFileName, outputData_reject};
    return decompose;
}

void
McCAD::IO::Manager::convert(McCAD::IO::InputConfig& inpufConfig){}
