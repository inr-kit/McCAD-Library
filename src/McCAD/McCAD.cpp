// C++
#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>
#include <ctime>
// McCAD
#include "info.hpp"
#include "inputconfig.hpp"
#include "inputdata.hpp"
#include "stepreader.hpp"
#include "stepwriter.hpp"
#include "decomposition.hpp"
#include "conversion.hpp"

int main (int argc, char* argv[]){
    auto timeStart{std::chrono::system_clock::now()},
         timeEnd{std::chrono::system_clock::now()};
    std::time_t timeStart_t = std::chrono::system_clock::to_time_t(timeStart);
    std::cerr << "Running McCAD v" << McCAD::Info::getMcCADVersion() << " / " <<
                 std::ctime(&timeStart_t) << std::endl;
    std::filesystem::path currentPath = std::filesystem::current_path();
    McCAD::IO::InputConfig inputConfig{currentPath};
    if (argc == 1){
        inputConfig.writeTemplate();
        std::cerr << "A template file, McCADInputConfig.txt, with run parameters "
                     "has been created in\n" << currentPath.string() << std::endl;
        timeEnd = std::chrono::system_clock::now();
    } else if(argc == 2) {
        if (std::string(argv[1]) == "help") {
            std::cout << "Usage:\n"
                         "   [ ] creates parameters file McCADInputConfig.txt\n"
                         "[read] Test loading the input STEP file\n"
                         " [run] Executes McCAD" << std::endl;
            timeEnd = std::chrono::system_clock::now();
        } else if (std::string(argv[1]) == "read") {
            inputConfig.readTemplate();
            // Load the input file.
            std::cout << "**************************" << std::endl;
            std::cout << "** Loading STEP file(s) **" << std::endl;
            std::cout << "**************************" << std::endl;
            for(int i = 0; i < inputConfig.inputFileNames.size(); ++i){
                inputConfig.inputFileName = inputConfig.inputFileNames[i];
                McCAD::IO::STEPReader reader{inputConfig};
            }
            timeEnd = std::chrono::system_clock::now();
        } else if (std::string(argv[1]) == "run") {
            inputConfig.readTemplate();
            bool decomposeCondition{inputConfig.decompose},
                 convertCondition{inputConfig.convert};
            if (decomposeCondition){
                bool rejectConversion;
                for(int i = 0; i < inputConfig.inputFileNames.size(); ++i){
                    inputConfig.inputFileName = inputConfig.inputFileNames[i];
                    std::cout << "> Processing " << inputConfig.inputFileName << std::endl;
                    // Load the input file.
                    std::cout << "***********************" << std::endl;
                    std::cout << "** Loading STEP file **" << std::endl;
                    std::cout << "***********************" << std::endl;
                    McCAD::IO::STEPReader reader{inputConfig};
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
                    inputConfig.outputFileName = inputConfig.decomposedFileNames[i];
                    McCAD::IO::STEPWriter{inputConfig, outputData_result};
                    inputConfig.outputFileName = inputConfig.rejectedFileNames[i];
                    McCAD::IO::STEPWriter{inputConfig, outputData_reject};
                    rejectConversion = outputData_reject.getSize() == 0 ? false : true;
                }
                if (convertCondition && !rejectConversion){
                    inputConfig.conversionFileNames = inputConfig.decomposedFileNames;
                    goto convert;
                } else if (rejectConversion)
                    std::cout << "Decomposition resulted in rejected solids."
                              << std::endl;
            } else if (convertCondition){
                inputConfig.conversionFileNames = inputConfig.inputFileNames;
                convert:;
                std::cout << "*************************" << std::endl;
                std::cout << "** Starting conversion **" << std::endl;
                std::cout << "*************************" << std::endl;
                McCAD::Conversion::Convert{inputConfig};
            };
            timeEnd = std::chrono::system_clock::now();
        } else goto usageError;
    } else {
        usageError:;
        std::cerr << "Usage Error!. Only [], [help], [read], or [run] are "
                     "acceptable arguments!" << std::endl;
        timeEnd = std::chrono::system_clock::now();
    }
    std::chrono::duration<double, std::ratio<60>> elapsed = timeEnd - timeStart;
    std::cout << "Execution time [m]: " << elapsed.count() << std::endl;
}
