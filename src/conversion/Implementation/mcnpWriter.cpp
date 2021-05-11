//C++
#include <filesystem>
#include <iostream>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <cmath>
// McCAD
#include "mcnpWriter.hpp"
#include "TaskQueue.hpp"
#include "mcnpExpressionGenerator.hpp"
#include "SurfaceUtilities.hpp"
// OCC
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>

McCAD::Conversion::MCNPWriter::MCNPWriter(const IO::InputConfig& inputConfig) :
    MCOutputFileName{inputConfig.MCOutputFileName}, volumesFileName{inputConfig.volumesFileName},
    startCellNum{inputConfig.startCellNum}, startSurfNum{inputConfig.startSurfNum},
    precision{inputConfig.precision}, maxLineWidth{inputConfig.maxLineWidth},
    voidGeneration{inputConfig.voidGeneration}, BVHVoid{inputConfig.BVHVoid}{
}

McCAD::Conversion::MCNPWriter::~MCNPWriter(){
}

void
McCAD::Conversion::MCNPWriter::operator()(
        const McCAD::Conversion::MCNPWriter::solidsList& solidObjList,
        const std::shared_ptr<VoidCell>& voidCell){
    processSolids(solidObjList);
    addUniqueSurfNumbers(solidObjList);
    createComponentMap(solidObjList);
    createVoidMap(voidCell);
    // Create output file stream and write cells, surfaces, and data cards.
    if(std::filesystem::exists(MCOutputFileName)){
        std::string oldFileName{"old_" + MCOutputFileName};
        std::rename(MCOutputFileName.c_str(), oldFileName.c_str());
    }
    if(std::filesystem::exists(volumesFileName)){
        std::string oldFileName{"old_" + volumesFileName};
        std::rename(volumesFileName.c_str(), oldFileName.c_str());
    }
    ofstream outputStream(MCOutputFileName.c_str()), volumeStream(volumesFileName.c_str());
    writeHeader(outputStream);
    writeCellCard(outputStream, volumeStream);
    writeVoidCard(outputStream);
    writeSurfCard(outputStream);
    writeDataCard(outputStream);
    outputStream.close();
    volumeStream.close();
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
                    uniqueSurfaces[surfNumber] = surface->accessSImpl()->surfExpr;
                    ++surfNumber;
                }
            } else if(surface->accessSImpl()->surfaceType == Tools::toTypeName(GeomAbs_Cylinder)){
                auto duplicateID = findDuplicate(surface, uniqueCylinders);
                if(duplicateID){
                    surface->accessSImpl()->uniqueID = *duplicateID;
                } else{
                    surface->accessSImpl()->uniqueID = surfNumber;
                    uniqueCylinders[surfNumber] = surface;
                    uniqueSurfaces[surfNumber] = surface->accessSImpl()->surfExpr;
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
                    uniqueSurfaces[surfNumber] = surface->accessSImpl()->surfExpr;
                    ++surfNumber;
                }
            }  else if(surface->accessSImpl()->surfaceType == Tools::toTypeName(GeomAbs_Cylinder)){
                auto duplicateID = findDuplicate(surface, uniqueCylinders);
                if(duplicateID){
                    surface->accessSImpl()->uniqueID = *duplicateID;
                } else{
                    surface->accessSImpl()->uniqueID = surfNumber;
                    uniqueCylinders[surfNumber] = surface;
                    uniqueSurfaces[surfNumber] = surface->accessSImpl()->surfExpr;
                    ++surfNumber;
                }
            }
        }
    }
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
McCAD::Conversion::MCNPWriter::addDaughterVoids(const std::shared_ptr<VoidCell>& voidCell){
    if(BVHVoid){
        // Add all inner and leaf nodes in the binary tree.
        if(voidCell->daughterVoidCells.size() > 0){
            // Add daughter void cells.
            for(const auto& daughter : voidCell->daughterVoidCells){
                std::tuple<Standard_Integer, Standard_Integer> voidDesignator =
                        std::make_tuple(daughter->depth, daughter->width);
                voidCellsMap[voidDesignator] = daughter;
                addDaughterVoids(daughter);
            }
        } else return;
    } else {
        // Add only leaf nodes.
        if(voidCell->daughterVoidCells.size() > 0){
            // Add daughter void cells.
            for(const auto& daughter : voidCell->daughterVoidCells){
                addDaughterVoids(daughter);
            }
        } else {
            // Reached a leaf node. Add void cell to map.
            std::tuple<Standard_Integer, Standard_Integer> voidDesignator =
                    std::make_tuple(voidCell->depth, voidCell->width);
            voidCellsMap[voidDesignator] = voidCell;
        }
    }
}

void
McCAD::Conversion::MCNPWriter::createVoidMap(
        const std::shared_ptr<VoidCell>& voidCell){
    // Add root void cell, also it is the graveyard.
    std::tuple<Standard_Integer, Standard_Integer> voidDesignator =
            std::make_tuple(voidCell->depth, voidCell->width);
    voidCellsMap[voidDesignator] = voidCell;
    if(voidGeneration) addDaughterVoids(voidCell);
    Standard_Integer voidSurfNumber = startSurfNum + uniqueSurfaces.size();
    for (const auto& member : voidCellsMap){
        MCNPExprGenerator{}.genVoidExpr(member.second);
        member.second->voidSurfNumber = voidSurfNumber;
        uniqueSurfaces[voidSurfNumber] = member.second->voidSurfExpr;
        ++voidSurfNumber;
    }
}

std::string
McCAD::Conversion::MCNPWriter::adjustLineWidth(const std::string& mainExpr,
                                               const std::string& bodyExpr){
    // Adjust cell solids expression to 80 columns max.
    std::string finalExpr = mainExpr;
    Standard_Integer continueSpacing = finalExpr.size() + 2;
    std::vector<std::string> splitExpr;
    boost::split(splitExpr, bodyExpr, [](char c) {return c == ' ';});
    Standard_Integer lineIndex{1};
    for(Standard_Integer i = 0; i < splitExpr.size(); ++i){
        if((finalExpr.size() + splitExpr[i].size()) > maxLineWidth*lineIndex){
            auto newSize = finalExpr.size() + continueSpacing;
            finalExpr += "\n";
            finalExpr.resize(newSize, *const_cast<char*>(" "));
            finalExpr += splitExpr[i];
            ++lineIndex;
        } else finalExpr += splitExpr[i];
        finalExpr += " ";
    }
    return finalExpr;
}

void
McCAD::Conversion::MCNPWriter::writeHeader(ofstream& outputStream){
    outputStream << "McCad v1.0L generated MC input files." <<
                    "\nc     * Material Cells ---- " << componentsMap.size() <<
                    "\nc     * Surfaces       ---- " << uniqueSurfaces.size() <<
                    "\nc     * Void cells     ---- " << voidCellsMap.size() << std::endl;
}

void
McCAD::Conversion::MCNPWriter::writeCellCard(ofstream& outputStream,
                                             ofstream& volumeStream){
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
            cellExpr += boost::str(boost::format(" %d %10.5f") % matID % matDensity);
            outputStream << "\nc * Density  : " << matDensity;
        }
        outputStream << "\nc ============" << std::endl;
        std::string cellSolidsExpr;
        Standard_Real componentVolume{0};
        for(Standard_Integer i = 0; i < member.second.size(); ++i){
            if(i == 0) cellSolidsExpr += " (";
            else cellSolidsExpr += " : (";
            MCNPExprGenerator{}.genCellExpr(member.second.at(i));
            cellSolidsExpr += member.second.at(i)->accessSImpl()->cellExpr;
            cellSolidsExpr += ")";
            componentVolume += member.second.at(i)->accessSImpl()->solidVolume;
        }
        cellSolidsExpr += " Imp:N=1 Imp:P=1";
        outputStream << adjustLineWidth(cellExpr, cellSolidsExpr) << std::endl;
        std::string componentData{boost::str(boost::format("%d %11.5f %s")
                                             % cellNumber
                                             % componentVolume
                                             % member.second.at(0)->accessSImpl()->solidName)};
        volumeStream << componentData << std::endl;
        ++cellNumber;
    }
}

void
McCAD::Conversion::MCNPWriter::writeVoidCard(ofstream& outputStream){
    outputStream << "c ==================== Void Cells ====================" << std::endl;
    Standard_Integer voidNumber = startCellNum + componentsMap.size();
    if(!voidGeneration) goto writeGraveYard;
    for(const auto& member : voidCellsMap){
        std::string voidExpr{boost::str(boost::format("%d") % voidNumber)};
        if (voidExpr.size() < 5) voidExpr.resize(5, *const_cast<char*>(" "));
        voidExpr += boost::str(boost::format(" %d") % 0);
        std::string voidSolidsExpr;
        voidSolidsExpr += boost::str(boost::format(" %d") % (-1 * member.second->voidSurfNumber));
        if(BVHVoid){
            if(member.second->daughterVoidCells.size() > 0){
                // Write inner nodes.
                for(const auto& daughterVoid : member.second->daughterVoidCells){
                    voidSolidsExpr += boost::str(boost::format(" %d") % daughterVoid->voidSurfNumber);
                }
            } else{
                // Write leaf node. Write complement of solids in the void cell.
                for(const Standard_Integer& solidID : member.second->solidIDList){
                    voidSolidsExpr += solidObjMap[solidID]->accessSImpl()->complimentExpr;
                }
            }
        } else{
            if(member.first == std::make_tuple(0,0)) continue;
            // Write leaf nodes only.
            for(const Standard_Integer& solidID : member.second->solidIDList){
                voidSolidsExpr += solidObjMap[solidID]->accessSImpl()->complimentExpr;
            }
        }
        voidSolidsExpr += " Imp:N=1 Imp:P=1";
        outputStream << adjustLineWidth(voidExpr, voidSolidsExpr) << std::endl;
        ++voidNumber;
    }
    writeGraveYard:;
    Standard_Integer voidSurfNumber{voidCellsMap[std::make_tuple(0,0)]->voidSurfNumber};
    std::string graveYardExpr{boost::str(boost::format("%d") % voidNumber)};
    if (graveYardExpr.size() < 5) graveYardExpr.resize(5, *const_cast<char*>(" "));
    graveYardExpr += boost::str(boost::format(" %d") % 0);
    graveYardExpr += boost::str(boost::format(" %d") % voidSurfNumber);
    graveYardExpr += " Imp:N=0 Imp:P=0 $Graveyard";
    outputStream << graveYardExpr << std::endl;
    uniqueSurfaces[voidSurfNumber] = voidCellsMap[std::make_tuple(0,0)]->voidSurfExpr;
    // Add cell to calculate volumes.
    std::string volumeCellExpr{boost::str(boost::format("%d") % (voidNumber + 1))};
    if (volumeCellExpr.size() < 5) volumeCellExpr.resize(5, *const_cast<char*>(" "));
    volumeCellExpr += boost::str(boost::format(" %d") % 0);
    volumeCellExpr += boost::str(boost::format(" -%d %d Imp:N=1 Imp:P=1")
                                 % (uniqueSurfaces.size() + startSurfNum)
                                 % voidSurfNumber);
    outputStream << "c " << volumeCellExpr << std::endl;
    std::string volumeCellGYExpr{boost::str(boost::format("%d") % (voidNumber + 2))};
    if (volumeCellGYExpr.size() < 5) volumeCellGYExpr.resize(5, *const_cast<char*>(" "));
    volumeCellGYExpr += boost::str(boost::format(" %d") % 0);
    volumeCellGYExpr += boost::str(boost::format(" %d Imp:N=0 Imp:P=0")
                                 % (uniqueSurfaces.size() + startSurfNum));
    outputStream << "c " << volumeCellGYExpr << std::endl;
}

void
McCAD::Conversion::MCNPWriter::writeSurfCard(ofstream& outputStream){
    outputStream << "\nc ==================== Surface Cards ====================" << std::endl;
    // Write all surfaces.
    for(const auto& surface : uniqueSurfaces){
        std::string surfExpr{boost::str(boost::format("%d") % surface.first)};
        if (surfExpr.size() < 5) surfExpr.resize(5, *const_cast<char*>(" "));
        outputStream << adjustLineWidth(surfExpr, surface.second) << std::endl;
    }
    // Add spherical surface to calculate the volumes.
    Standard_Real xExt, yExt, zExt;
    auto& graveyard = voidCellsMap[std::make_tuple(0,0)];
    xExt = std::get<2>(graveyard->xAxis) - std::get<0>(graveyard->xAxis);
    yExt = std::get<2>(graveyard->yAxis) - std::get<0>(graveyard->yAxis);
    zExt = std::get<2>(graveyard->zAxis) - std::get<0>(graveyard->zAxis);
    radius = std::max(std::max(xExt, yExt), zExt);
    std::string surfExpr{boost::str(boost::format("%d") % (startSurfNum + uniqueSurfaces.size()))};
    if (surfExpr.size() < 5) surfExpr.resize(5, *const_cast<char*>(" "));
    std::string surfCoord{boost::str(boost::format("S %6.3f %6.3f %6.3f %6.3f")
                            % std::get<1>(graveyard->xAxis)
                            % std::get<1>(graveyard->yAxis)
                            % std::get<1>(graveyard->zAxis)
                            % radius)};
    outputStream << "c " << adjustLineWidth(surfExpr, surfCoord) << std::endl;
}

void
McCAD::Conversion::MCNPWriter::writeDataCard(ofstream& outputStream){
    outputStream << "\nc ==================== Data Cards ====================" << std::endl;
    // add tallies and source to calculate volumes.
    outputStream << "Mode N" << "\nVoid" << "\nNPS 1e9" << "\nPRDMP 1e8 1e8 j 1 j" << std::endl;
    std::string sourceExpr{boost::str(
                    boost::format("SDEF ERG=14.1 $ SUR=%d PAR=1 NRM=-1 WGT=%6.3f $Pi*r^2")
                    % (startSurfNum + uniqueSurfaces.size()) % (M_PI * std::pow(radius, 2)))};
    outputStream << sourceExpr << std::endl;
    std::string volTallyExpr{boost::str(boost::format("F4:N %d %di %d")
                                        % startCellNum
                                        % (componentsMap.size() - 2)
                                        % (startCellNum + componentsMap.size() - 1))};
    volTallyExpr += boost::str(boost::format("\nc SD4 1 %dr") % (componentsMap.size() - 1));
    outputStream << "c " << volTallyExpr << std::endl;
}

