//C++
#include <filesystem>
#include <iostream>
// McCAD
#include "MCNPWriter.hpp"

McCAD::Conversion::MCNPWriter::MCNPWriter(const std::string& MCOutputFileName,
                                          const int& startCellNum,
                                          const int& startSurfNum) :
    MCOutputFileName{MCOutputFileName}, startCellNum{startCellNum},
    startSurfNum{startSurfNum}{}

McCAD::Conversion::MCNPWriter::~MCNPWriter(){}

void
McCAD::Conversion::MCNPWriter::processSolids(
        const McCAD::Conversion::MCNPWriter::solidsList& solidObjList){
    if(std::filesystem::exists(MCOutputFileName)){
        std::string oldFileName{"old_" + MCOutputFileName};
        std::rename(MCOutputFileName.c_str(), oldFileName.c_str());
    }
    ofstream outputStream(MCOutputFileName.c_str());
    // Print header
    writeHeader(outputStream);
    outputStream.close();
}

void
McCAD::Conversion::MCNPWriter::processVoids(const std::shared_ptr<VoidCell>& voidCell){
    //
}

void
McCAD::Conversion::MCNPWriter::writeHeader(ofstream& outputStream){
    outputStream <<"McCad v1.oL generated MC input files."
                   "\nc ======================================="
                   "\nc     * Material cells  ---- " <<
                   "\nc     * Void cells  -------- " <<
                   "\nc     * Surfaces    -------- " <<
                   "\nc     * Outer Void  -------- " <<
                   "\nc =======================================";
}

