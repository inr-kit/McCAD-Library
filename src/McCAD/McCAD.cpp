// C++
#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>
// McCAD
#include "inputconfig.hpp"
#include "inputdata.hpp"
#include "stepreader.hpp"
#include "stepwriter.hpp"
#include "decomposition.hpp"
#include "conversion.hpp"

int main (int argc, char* argv[]){
    auto timeStart{std::chrono::high_resolution_clock::now()},
         timeEnd{std::chrono::high_resolution_clock::now()};
    std::cerr << "Running McCAD v1.0L" << std::endl;
    std::filesystem::path currentPath = std::filesystem::current_path();
    McCAD::IO::InputConfig inputConfig{currentPath};
    if (argc == 1){
        inputConfig.writeTemplate();
        std::cerr << "A template file, McCADInputConfig.txt, with run parameters"
                     "has been created in\n" << std::string(currentPath) << std::endl;
        timeEnd = std::chrono::high_resolution_clock::now();
    } else if(argc == 2) {
        if (std::string(argv[1]) == "help") {
            std::cout << "Usage:\n"
                         "   [ ] creates parameters file McCADInputConfig.txt\n"
                         "[read] Test loading input STEP file\n"
                         " [run] Executes McCAD" << std::endl;
            timeEnd = std::chrono::high_resolution_clock::now();
        } else if (std::string(argv[1]) == "read") {
            inputConfig.readTemplate();
            // Load the input file.
            std::cout << "***********************" << std::endl;
            std::cout << "** Loading STEP file **" << std::endl;
            std::cout << "***********************" << std::endl;
            McCAD::IO::STEPReader reader{inputConfig.inputFileName};
            timeEnd = std::chrono::high_resolution_clock::now();
        } else if (std::string(argv[1]) == "run") {
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
                McCAD::Decomposition::Decompose decompose{inputData, inputConfig};
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
                    inputConfig.conversionFileName = inputConfig.resultFileName;
                    goto convert;
                } else if (rejectConversion)
                    std::cout << "Decomposition resulted in rejected solids, please "
                                 "check the solids and then run conversion!"
                              << std::endl;
            } else if (convertCondition){
                inputConfig.conversionFileName = inputConfig.inputFileName;
                convert:;
                std::cout << "*************************" << std::endl;
                std::cout << "** Starting conversion **" << std::endl;
                std::cout << "*************************" << std::endl;
                McCAD::Conversion::Convert{inputConfig};
            };
            timeEnd = std::chrono::high_resolution_clock::now();
        } else goto usageError;
    } else {
        usageError:;
        std::cerr << "Usage Error!. Only [], [help], [read], or [run] are "
                     "acceptable arguments!" << std::endl;
        timeEnd = std::chrono::high_resolution_clock::now();
    }
    std::chrono::duration<double, std::milli> elapsed = timeEnd - timeStart;
    std::cout << "Execuion time [ms]: " << elapsed.count() << std::endl;
}
