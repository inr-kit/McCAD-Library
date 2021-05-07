//C++
#include <filesystem>
#include <iostream>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
// McCAD
#include "mcnpWriter.hpp"
#include "TaskQueue.hpp"
#include "mcnpExpressionGenerator.hpp"
#include "SurfaceUtilities.hpp"

McCAD::Conversion::MCNPWriter::MCNPWriter(const IO::InputConfig& inputConfig) :
    MCOutputFileName{inputConfig.MCOutputFileName}, startCellNum{inputConfig.startCellNum},
    startSurfNum{inputConfig.startSurfNum}, precision{inputConfig.precision},
    maxLineWidth{inputConfig.maxLineWidth}, voidGeneration{inputConfig.voidGeneration},
    BVHVoid{inputConfig.BVHVoid}{
}

McCAD::Conversion::MCNPWriter::~MCNPWriter(){
}

void
McCAD::Conversion::MCNPWriter::operator()(
        const McCAD::Conversion::MCNPWriter::solidsList& solidObjList,
        const std::shared_ptr<VoidCell>& voidCell){
    processSolids(solidObjList);
    Standard_Integer totalSurfNumber = addUniqueSurfNumbers(solidObjList) - 1;
    //processVoids(voidCell);
    createComponentMap(solidObjList);
    // Create output file stream and write cells, surfaces, and data cards.
    if(std::filesystem::exists(MCOutputFileName)){
        std::string oldFileName{"old_" + MCOutputFileName};
        std::rename(MCOutputFileName.c_str(), oldFileName.c_str());
    }
    ofstream outputStream(MCOutputFileName.c_str());
    writeHeader(outputStream, componentsMap.size(), totalSurfNumber);
    writeCellCard(outputStream);
    writeVoidCard(outputStream, voidCell);
    writeSurfCard(outputStream);
    writeDataCard(outputStream);
    outputStream.close();
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
            } else if(surface->accessSImpl()->surfaceType == Tools::toTypeName(GeomAbs_Cylinder)){
                auto duplicateID = findDuplicate(surface, uniqueCylinders);
                if(duplicateID){
                    surface->accessSImpl()->uniqueID = *duplicateID;
                } else{
                    surface->accessSImpl()->uniqueID = surfNumber;
                    uniqueCylinders[surfNumber] = surface;
                    ++surfNumber;
                }
            }
        }
        for(const auto& surface : solidObj->accessSImpl()->unionList){
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
            }  else if(surface->accessSImpl()->surfaceType == Tools::toTypeName(GeomAbs_Cylinder)){
                auto duplicateID = findDuplicate(surface, uniqueCylinders);
                if(duplicateID){
                    surface->accessSImpl()->uniqueID = *duplicateID;
                } else{
                    surface->accessSImpl()->uniqueID = surfNumber;
                    uniqueCylinders[surfNumber] = surface;
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
McCAD::Conversion::MCNPWriter::createComponentMap(
        const McCAD::Conversion::MCNPWriter::solidsList& solidObjList){
    // Create map of unique components. All solids has an "originalID", this is
    // order of the parent components/solid in list of solids read from step file.
    for(const auto& solidObj : solidObjList){
        componentsMap[solidObj->accessSImpl()->originalID].push_back(solidObj);
        solidObjMap[solidObj->accessSImpl()->solidID] = solidObj;
    }
}

void
McCAD::Conversion::MCNPWriter::writeHeader(ofstream& outputStream,
                                           const Standard_Integer& numCells,
                                           const Standard_Integer& totalSurfNumber){
    outputStream << "McCad v1.0L generated MC input files." <<
                    "\nc     * Cells       ---- " << numCells <<
                    "\nc     * Surfaces    ---- " << totalSurfNumber <<
                    "\nc     * Void cells  ---- " << std::endl;
}

void
McCAD::Conversion::MCNPWriter::writeCellCard(ofstream& outputStream){
    outputStream << "c ==================== Cell Cards ====================" << std::endl;
    Standard_Integer cellNumber = startCellNum;
    // Need to loop over all solids in a compSolid, write header with maerial,
    // component name, cell range, etc. Adjust width of expression
    for(const auto& member : componentsMap){
        outputStream << "c ============" <<
                        "\nc * Component: " << member.second.at(0)->accessSImpl()->solidName <<
                        "\nc * Subsolids: " << member.second.size();
        std::string cellExpr{boost::str(boost::format("%d") % cellNumber)};
        if (cellExpr.size() < 5) cellExpr.resize(5, *const_cast<char*>(" "));
        // Add material from first element in vector.
        auto matID = member.second[0]->accessSImpl()->matID;
        auto matDensity = member.second[0]->accessSImpl()->matDensity;
        if(matID == 0){
            cellExpr += boost::str(boost::format(" %d") % matID);
            outputStream << "\nc * Material : " << matID;
        } else{
            cellExpr += boost::str(boost::format(" %d $10.5f") % matID % matDensity);
            outputStream << "\nc * Density  : " << matDensity;
        }
        outputStream << "\nc ============" << std::endl;
        Standard_Integer continueSpacing = cellExpr.size() + 2;
        std::string cellSolidsExpr;
        for(Standard_Integer i = 0; i < member.second.size(); ++i){
            if(i == 0) cellSolidsExpr += " (";
            else cellSolidsExpr += " : (";
            MCNPExprGenerator{}.genCellExpr(member.second.at(i));
            cellSolidsExpr += member.second.at(i)->accessSImpl()->cellExpr;
            cellSolidsExpr += ")";
        }
        cellSolidsExpr += " Imp:N=1 Imp:P=1";
        // Adjust cell solids expression to 80 columns max.
        std::vector<std::string> splitExpr;
        boost::split(splitExpr, cellSolidsExpr, [](char c) {return c == ' ';});
        Standard_Integer lineIndex{1};
        for(Standard_Integer i = 0; i < splitExpr.size(); ++i){
            if((cellExpr.size() + splitExpr[i].size()) > maxLineWidth*lineIndex){
                auto newSize = cellExpr.size() + continueSpacing;
                cellExpr += "\n";
                cellExpr.resize(newSize, *const_cast<char*>(" "));
                cellExpr += splitExpr[i];
                ++lineIndex;
            } else cellExpr += splitExpr[i];
            cellExpr += " ";
        }
        outputStream << cellExpr << std::endl;
        ++cellNumber;
    }
}

void
McCAD::Conversion::MCNPWriter::writeVoidCard(ofstream& outputStream,
                                             const std::shared_ptr<VoidCell>& voidCell){
    outputStream << "c ==================== Void Cells ====================" << std::endl;
    Standard_Integer voidNumber = startCellNum + componentsMap.size();
    if(!voidGeneration){
        std::string graveYardExpr{boost::str(boost::format("%d") % voidNumber)};
        if (graveYardExpr.size() < 5) graveYardExpr.resize(5, *const_cast<char*>(" "));
        graveYardExpr += boost::str(boost::format(" %d") % 0);
        Standard_Integer continueSpacing = graveYardExpr.size() + 2;
        std::string voidSolidsExpr;
        for(const auto& solidID : voidCell->solidIDList){
            voidSolidsExpr += " (";
            voidSolidsExpr += solidObjMap[solidID]->accessSImpl()->complimentExpr;
            voidSolidsExpr += ")";
        }
        voidSolidsExpr += " Imp:N=1 Imp:P=1";
        // Adjust cell solids expression to 80 columns max.
        std::vector<std::string> splitExpr;
        boost::split(splitExpr, voidSolidsExpr, [](char c) {return c == ' ';});
        Standard_Integer lineIndex{1};
        for(Standard_Integer i = 0; i < splitExpr.size(); ++i){
            if((graveYardExpr.size() + splitExpr[i].size()) > maxLineWidth*lineIndex){
                auto newSize = graveYardExpr.size() + continueSpacing;
                graveYardExpr += "\n";
                graveYardExpr.resize(newSize, *const_cast<char*>(" "));
                graveYardExpr += splitExpr[i];
                ++lineIndex;
            } else graveYardExpr += splitExpr[i];
            graveYardExpr += " ";
        }
        outputStream << graveYardExpr << std::endl;
    }
}

void
McCAD::Conversion::MCNPWriter::writeSurfCard(ofstream& outputStream){
    outputStream << "\nc ==================== Surface Cards ====================" << std::endl;
    // create one big list of tuples <surfaceNum, surfExpr>, order by surfNum, write all
    if (uniquePlanes.size() > 1){
        for(const auto& member : uniquePlanes){
            uniqueSurfaces[member.first] = member.second->accessSImpl()->surfExpr;
        }
    }
    // Write all surfaces.
    for(const auto& surface : uniqueSurfaces){
        std::string surfExpr{boost::str(boost::format("%d") % surface.first)};
        if (surfExpr.size() < 5) surfExpr.resize(5, *const_cast<char*>(" "));
        surfExpr += surface.second;
        outputStream << surfExpr << std::endl;
    }
}

void
McCAD::Conversion::MCNPWriter::writeDataCard(ofstream& outputStream){
    outputStream << "\nc ==================== Data Cards ====================" << std::endl;
    // add tallies and source to calculate volumes.
}

