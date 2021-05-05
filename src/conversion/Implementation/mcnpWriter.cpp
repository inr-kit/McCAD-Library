//C++
#include <filesystem>
#include <iostream>
// McCAD
#include "mcnpWriter.hpp"
#include "TaskQueue.hpp"
#include "mcnpExpressionGenerator.hpp"
#include "SurfaceUtilities.hpp"

McCAD::Conversion::MCNPWriter::MCNPWriter(const std::string& MCOutputFileName,
                                          const Standard_Integer& startCellNum,
                                          const Standard_Integer& startSurfNum,
                                          const Standard_Real& precision) :
    MCOutputFileName{MCOutputFileName}, startCellNum{startCellNum},
    startSurfNum{startSurfNum}, precision{precision}{
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
    Standard_Integer totalSurfNumber = addUniqueSurfNumbers(solidObjList);
    // Create output file stream and write cells, surfaces, and data cards.
    ofstream outputStream(MCOutputFileName.c_str());
    writeHeader(outputStream, solidObjList.size(), totalSurfNumber);
    writeCellCard(outputStream, solidObjList);
    writeSurfCard(outputStream);
    writeDataCard(outputStream);
    outputStream.close();
}

void
McCAD::Conversion::MCNPWriter::processVoids(
        const std::shared_ptr<VoidCell>& voidCell){
    //
}

Standard_Integer
McCAD::Conversion::MCNPWriter::addUniqueSurfNumbers(
        const McCAD::Conversion::MCNPWriter::solidsList& solidObjList){
    Standard_Integer surfNumber = startSurfNum;
    for(const auto& solidObj : solidObjList){
        for(const auto& surface : solidObj->accessSImpl()->intersectionList){
            if(surface->accessSImpl()->surfaceType == Tools::toTypeName(GeomAbs_Plane)){
                // Surface is plane. Compare to unique planes list and create a unique ID.
                auto duplicateID = findDuplicate(surface, uniquePlanes);
                if(duplicateID){
                    // An ID of duplicate surfae is returned.
                    surface->accessSImpl()->uniqueID = *duplicateID;
                } else{
                    surface->accessSImpl()->uniqueID = surfNumber;
                    uniquePlanes[surfNumber] = surface;
                    ++surfNumber;
                }
            }
        }
    }
    return surfNumber;
}

std::optional<Standard_Integer>
McCAD::Conversion::MCNPWriter::findDuplicate(
        const std::shared_ptr<Geometry::BoundSurface>& surface,
        McCAD::Conversion::MCNPWriter::surfacesMap& uniqueMap){
    // Compare both surface sybmol and parameters.
    if(uniqueMap.size() == 0) return std::nullopt;
    for (const auto& member : uniqueMap){
        if (surface->accessSImpl()->surfSymb == member.second->accessSImpl()->surfSymb){
            // compare parameters.
            Standard_Boolean equalParmts{Standard_True};
            for (Standard_Integer i = 0; i < surface->accessSImpl()->surfParameters.size(); ++i){
                if (std::abs(surface->accessSImpl()->surfParameters[i] -
                             member.second->accessSImpl()->surfParameters[i]) > precision)
                    equalParmts = Standard_False;
            }
            if (equalParmts) return member.first;
        }
    }
    return std::nullopt;
}

void
McCAD::Conversion::MCNPWriter::writeHeader(ofstream& outputStream,
                                           const Standard_Integer& numCells,
                                           const Standard_Integer& totalSurfNumber){
    outputStream << "McCad v1.0L generated MC input files."
                    "\nc ======================================="
                    "\nc     * Cells       ---- " << numCells <<
                    "\nc     * Surfaces    ---- " << totalSurfNumber <<
                    "\nc     * Void cells  ---- " <<
                    "\nc =======================================" << std::endl;
}

void
McCAD::Conversion::MCNPWriter::writeCellCard(
        ofstream& outputStream,
        const McCAD::Conversion::MCNPWriter::solidsList& solidObjList){
    outputStream << "c ========== Cell Cards ==========" << std::endl;
    Standard_Integer cellNumber = startCellNum;
    for(const auto& solidObj : solidObjList){
        MCNPExprGenerator{}.genCellExpr(solidObj, cellNumber);
        outputStream << solidObj->accessSImpl()->cellExpr << std::endl;
        ++cellNumber;
    }
}

// create a map of unique surfaces and compare new surfaces to it. write only unique ones.
void
McCAD::Conversion::MCNPWriter::writeSurfCard(ofstream& outputStream){
    outputStream << "\nc ========== Surface Cards ==========" << std::endl;
    // create one big list of tuples <surfaceNum, surfExpr>, order by surfNum, write all
    if (uniquePlanes.size() > 1){
        for(const auto& member : uniquePlanes){
            uniqueSurfaces[member.first] = member.second->accessSImpl()->surfExpr;
        }
    }
    // Write all surfaces.
    for(const auto& surface : uniqueSurfaces){
       outputStream << std::to_string(surface.first);
       outputStream << " " << surface.second << std::endl;
    }
}

void
McCAD::Conversion::MCNPWriter::writeDataCard(ofstream& outputStream){
    outputStream << "\nc ========== Data Cards ==========" << std::endl;
}

