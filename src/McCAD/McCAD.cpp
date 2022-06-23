// C++
#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>
#include <ctime>
#include <list>
#include <algorithm>
// McCAD
#include "info.hpp"
#include "inputconfig.hpp"
#include "inputdata.hpp"
#include "stepreader.hpp"
#include "stepwriter.hpp"
#include "decomposition.hpp"
#include "conversion.hpp"

/** ********************************************************************
* @brief    The main function that executes McCAD per user arguments.
* @param    argc is the number of arguments entered.
* @param    argv is a list of arguments: [], [help], [read], or [run].
* @return   int
* @date     01/01/2021
* @modified 09/06/2022
* @author   Moataz Harb
* **********************************************************************/
int main (int argc, char* argv[]){
    auto timeStart{std::chrono::system_clock::now()},
         timeEnd{std::chrono::system_clock::now()};
    std::time_t timeStart_t = std::chrono::system_clock::to_time_t(timeStart);
    std::cout << "Running McCAD v" << McCAD::Info::McCADVersion << " / " <<
                 std::ctime(&timeStart_t) << std::endl;
    std::filesystem::path currentPath = std::filesystem::current_path();
    McCAD::IO::InputConfig inputConfig{currentPath};
    if (argc == 1){
        // No arguments given, write the config file to desk.
        inputConfig.writeTemplate();
        std::cout << "A template file, McCADInputConfig.i, with run parameters "
                     "has been created in\n" << currentPath.string() << std::endl;
        timeEnd = std::chrono::system_clock::now();
    } else if(argc == 2) {
      std::string arg1 = std::string(argv[1]);
      std::transform(arg1.begin(), arg1.end(), arg1.begin(),
                     [](unsigned char c) { return std::tolower(c); });
      if (arg1 == "help") {
            // "help" used as argument, print out the available arguments to screen.
            std::cout << "Usage, case insensitive:\n"
                         "   [ ] Creates parameters file McCADInputConfig.i\n"
                         "[help] Prints out this list\n"
                         "[read] Tests loading the input STEP file(s)\n"
                         " [run] Executes McCAD" << std::endl;
            timeEnd = std::chrono::system_clock::now();
        } else if (arg1 == "read") {
            // "read" used as argument, load the input STEP file[s].
            inputConfig.readTemplate();
            std::cout << "**************************" << std::endl;
            std::cout << "** Loading STEP file(s) **" << std::endl;
            std::cout << "**************************" << std::endl;
            // Loop over the list of file names and load them consequetively.
            for(int i = 0; i < inputConfig.inputFileNames.size(); ++i){
                inputConfig.inputFileName = inputConfig.inputFileNames[i];
                std::cout << "> Processing " << inputConfig.inputFileName << std::endl;
                McCAD::IO::STEPReader reader{inputConfig};
            }
            timeEnd = std::chrono::system_clock::now();
        } else if (arg1 == "run") {
            // "run" used as argument, read the config file and then run McCAD.
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
                    // Set the condition on conversion.
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
