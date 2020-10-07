// C++
#include <iostream>
#include <string>
#include <chrono>
#include <filesystem>

// McCAD
#include "inputdata.hpp"
#include "Inputconfig.hpp"
#include "stepreader.hpp"
#include "stepwriter.hpp"
#include "decomposition.hpp"

int main (int argc, char *argv[]){
    std::string currentPath = std::filesystem::current_path();
    McCAD::IO::InputConfig inpufConfig{currentPath};
    if (argc == 1){
        std::cerr << "Usage: A template file with run parameters will be "
                     "created!" << std::endl;
        inpufConfig.writeTemplate();
    } else if(argc == 2) {
        if (std::string(argv[1]) == "help") {
            std::cout << "Usage:   < >: create parameters file McCADInputConfig.txt\n"
                         "       <run>: execute McCAD" << std::endl;
        } else if (std::string(argv[1]) == "run") {
        std::cerr << "Usage: Running McCAD v1.0L!" << std::endl;
        inpufConfig.readTemplate();

        auto start = std::chrono::high_resolution_clock::now();
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
        std::cout << "****************************" << std::endl;
        std::cout << "**  Saving to STEP file   **" << std::endl;
        std::cout << "****************************" << std::endl;
        McCAD::IO::STEPWriter{inpufConfig.resultFileName, outputData_result};
        McCAD::IO::STEPWriter{inpufConfig.rejectFileName, outputData_reject};

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        std::cout << "Elapsed time: " << elapsed.count() << " ms" << std::endl;}
    } else std::cerr << "Usage: only [] or [run] are acceptable arguments!"
                     << std::endl;
    return 0;
}
