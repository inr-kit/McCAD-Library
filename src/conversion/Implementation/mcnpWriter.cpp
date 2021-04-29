//C++
#include <filesystem>
#include <iostream>
// McCAD
#include "mcnpWriter.hpp"
#include "TaskQueue.hpp"
#include "mcnpExpressionGenerator.hpp"

McCAD::Conversion::MCNPWriter::MCNPWriter(const std::string& MCOutputFileName,
                                          const int& startCellNum,
                                          const int& startSurfNum) :
    MCOutputFileName{MCOutputFileName}, startCellNum{startCellNum},
    startSurfNum{startSurfNum}{
}

McCAD::Conversion::MCNPWriter::~MCNPWriter(){
}

void
McCAD::Conversion::MCNPWriter::processSolids(
        const McCAD::Conversion::MCNPWriter::solidsList& solidObjList){
    TaskQueue<Policy::Parallel> taskQueue;
    for(const auto& solidObj : solidObjList){
        taskQueue.submit([this, solidObj](){
            MCNPExprGenerator{solidObj};
        });
    }
    taskQueue.complete();
    if(std::filesystem::exists(MCOutputFileName)){
        std::string oldFileName{"old_" + MCOutputFileName};
        std::rename(MCOutputFileName.c_str(), oldFileName.c_str());
    }
    ofstream outputStream(MCOutputFileName.c_str());
    // Print header
    writeHeader(outputStream);
    writeCellCard(outputStream, solidObjList);
    outputStream.close();
}

void
McCAD::Conversion::MCNPWriter::processVoids(
        const std::shared_ptr<VoidCell>& voidCell){
    //
}

void
McCAD::Conversion::MCNPWriter::writeHeader(ofstream& outputStream){
    outputStream << "McCad v1.oL generated MC input files."
                    "\nc ======================================="
                    "\nc     * Cells       ---- " <<
                    "\nc     * Surfaces    ---- " <<
                    "\nc     * Void cells  ---- " <<
                    "\nc =======================================";
}

void
McCAD::Conversion::MCNPWriter::writeCellCard(
        ofstream& outputStream,
        const McCAD::Conversion::MCNPWriter::solidsList& solidObjList){
    for(const auto& solidObj : solidObjList){
        if(solidObj->accessSImpl()->planesList.size() > 0){
            outputStream << "\nSolid:";
            for (const auto& plSurface : solidObj->accessSImpl()->planesList){
                outputStream << "\n" << plSurface->accessSImpl()->surfExpr
                             << "\nsense: " << plSurface->accessSImpl()->surfSense
                             << "\n";
            }
        }
    }
}
