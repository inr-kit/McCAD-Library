// C++
#include <iostream>
#include <string>
// McCAD
#include "inputdata.hpp"
#include "stepreader.hpp"
#include "stepwriter.hpp"
#include "decomposition.hpp"

int main (){
    std::string inputFileName, resultFileName, rejectFileName;
    std::cout << "Specify the path to the input step file: " << std::endl;
    getline(std::cin, inputFileName);
    std::cout << "Specify the path to save the result solids step file: " <<
                 std::endl;
    getline(std::cin, resultFileName);
    std::cout << "Specify the path to save the rejected solids step file: " <<
                 std::endl;
    getline(std::cin, rejectFileName);
    
    // Load the input file.
    std::cout << "***************************" << std::endl;
    std::cout << "** Loading the STEP file **" << std::endl;
    std::cout << "***************************" << std::endl;
    McCAD::IO::STEPReader reader{inputFileName};
    auto inputData = reader.getInputData();

    // Start decomposition.
    std::cout << "****************************" << std::endl;
    std::cout << "** Starting decomposition **" << std::endl;
    std::cout << "****************************" << std::endl;
    McCAD::Decomposition::Decompose decompose{inputData};
    auto outputData_result = decompose.getResultSolids();
    auto outputData_reject = decompose.getRejectedSolids();

    // Write output STEP files.
    std::cout << "****************************" << std::endl;
    std::cout << "**  Saving to STEP file   **" << std::endl;
    std::cout << "****************************" << std::endl;
    McCAD::IO::STEPWriter{resultFileName, outputData_result};
    McCAD::IO::STEPWriter{rejectFileName, outputData_reject};

    return 0;
}
