// C++
#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>
#include <sstream>
// McCAD
//#include "inputconfig.hpp"
#include "inputdata.hpp"
//#include "stepreader.hpp"
//#include "stepwriter.hpp"
//#include "stlwriter.hpp"
//#include "decomposition.hpp"
//#include "conversion.hpp"

int main (int argc, char* argv[]){
    std::stringstream strStream;
    strStream << std::filesystem::current_path();
    std::string currentPath = strStream.str();
    std::cout << "Hello!. The current path is: " << currentPath << std::endl;
    /*
    McCAD::IO::InputConfig inputConfig{currentPath};
    if (argc == 1){
        inputConfig.writeTemplate();
        std::cerr << "Usage: A template file with run parameters has been "
                     "created!" << std::endl;
    } else if(argc == 2) {
        if (std::string(argv[1]) == "help") {
            std::cout << "Usage:   [ ]: creates parameters file McCADInputConfig.txt\n"
                         "       [run]: executes McCAD" << std::endl;
        } else if (std::string(argv[1]) == "run") {
            std::cerr << "Usage: Running McCAD v1.0L!" << std::endl;
            auto start = std::chrono::high_resolution_clock::now();
            inputConfig.readTemplate();
            bool decomposeCondition{inputConfig.decompose},
            convertCondition{inputConfig.convert};
            if (decomposeCondition){
                // Load the input file.
                std::cout << "***********************" << std::endl;
                std::cout << "** Loading STEP file **" << std::endl;
                std::cout << "***********************" << std::endl;
                McCAD::IO::STEPReader reader{inputConfig.inputFileName};
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
                McCAD::IO::STEPWriter{inputConfig.resultFileName, outputData_result};
                McCAD::IO::STEPWriter{inputConfig.rejectFileName, outputData_reject};
                bool rejectConversion = outputData_reject.getSize() == 0 ? false
                                                                         : true;
                if (convertCondition && !rejectConversion){
                    std::cout << "*************************" << std::endl;
                    std::cout << "** Starting conversion **" << std::endl;
                    std::cout << "*************************" << std::endl;
                    // Save solids to STL file
                    McCAD::IO::STLWriter{inputConfig.conversionFileName, outputData_result};
                    McCAD::Conversion::Convert{inputConfig};
                } else if (rejectConversion)
                    std::cout << "Decomposition resulted in rejected solids, please "
                                 "check the solids and then run conversion!"
                              << std::endl;
            } else if (convertCondition){
                std::cout << "*************************" << std::endl;
                std::cout << "** Starting conversion **" << std::endl;
                std::cout << "*************************" << std::endl;
                McCAD::Conversion::Convert{inputConfig};
            };
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;
            std::cout << "Execuion time [ms]: " << elapsed.count() << std::endl;}
    } else std::cerr << "Usage: only [] or [help] or [run] are acceptable arguments!"
                     << std::endl;*/
}
