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
    writeHeader(outputStream, solidObjList);
    writeCellCard(outputStream, solidObjList);
    writeSurfCard(outputStream, solidObjList);
    writeDataCard(outputStream);
    outputStream.close();
}

void
McCAD::Conversion::MCNPWriter::processVoids(
        const std::shared_ptr<VoidCell>& voidCell){
    //
}

void
McCAD::Conversion::MCNPWriter::writeHeader(
        ofstream& outputStream,
        const McCAD::Conversion::MCNPWriter::solidsList& solidObjList){
    outputStream << "McCad v1.0L generated MC input files."
                    "\nc ======================================="
                    "\nc     * Cells       ---- " << solidObjList.size() <<
                    "\nc     * Void cells  ---- " <<
                    "\nc =======================================" << std::endl;
}

void
McCAD::Conversion::MCNPWriter::writeCellCard(
        ofstream& outputStream,
        const McCAD::Conversion::MCNPWriter::solidsList& solidObjList){
    outputStream << "c ========== Cell Cards ==========" << std::endl;
    Standard_Integer cellNumber = startCellNum;
    Standard_Integer surfNumber = startSurfNum;
    for(const auto& solidObj : solidObjList){
        MCNPExprGenerator{}.genCellExpr(solidObj, cellNumber, surfNumber);
        outputStream << solidObj->accessSImpl()->cellExpr << std::endl;
        ++cellNumber;
        surfNumber += solidObj->accessSImpl()->surfacesList.size();
    }
}

void
McCAD::Conversion::MCNPWriter::writeSurfCard(
        ofstream& outputStream,
        const McCAD::Conversion::MCNPWriter::solidsList& solidObjList){
    outputStream << "\nc ========== Surface Cards ==========" << std::endl;
    for(const auto& solidObj : solidObjList){
        for (const auto& member : solidObj->accessSImpl()->surfacesList){
            outputStream << std::to_string(member.first);
            outputStream << " " << member.second->accessSImpl()->surfExpr << std::endl;
        }
    }
}

void
McCAD::Conversion::MCNPWriter::writeDataCard(ofstream& outputStream){
    outputStream << "\nc ========== Data Cards ==========" << std::endl;
}
