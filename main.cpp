// C++
#include <iostream>
#include <string>
// McCAD
#include "inputdata.hpp"
#include "stepreader.hpp"
#include "stepwriter.hpp"
#include "decomposition.hpp"

int main (){
    std::string fileName;
    std::cout << "Specify the path to the step file: " << std::endl;
    getline(std::cin, fileName);
    
    // Load the input file.
    std::cout << "***************************" << std::endl;
    std::cout << "** Loading the STEP file **" << std::endl;
    std::cout << "***************************" << std::endl;
    McCAD::IO::STEPReader reader{fileName};
    auto inputData = reader.getInputData();

    // Start decomposition.
    std::cout << "****************************" << std::endl;
    std::cout << "** Starting decomposition **" << std::endl;
    std::cout << "****************************" << std::endl;
    McCAD::Decomposition::Decompose decompose{inputData};
    auto outputData = decompose.getResultSolids();

    // Erite output STEP file.
    std::cout << "****************************" << std::endl;
    std::cout << "** Saving to STEP file **" << std::endl;
    std::cout << "****************************" << std::endl;
    McCAD::IO::STEPWriter writer{fileName, outputData};
    
    return 0;
}
