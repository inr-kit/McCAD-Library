//C++
#include <filesystem>
#include <iostream>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <cmath>
#include <chrono>
#include <ctime>
// McCAD
#include "info.hpp"
#include "mcnpWriter.hpp"
#include "TaskQueue.hpp"
#include "mcnpExpressionGenerator.hpp"
#include "SurfaceUtilities.hpp"
#include "AssistSurfaceGenerator.hpp"

McCAD::Conversion::MCNPWriter::MCNPWriter(const IO::InputConfig& inputConfig) :
    inputConfig{inputConfig}, scalingFactor{1.0/inputConfig.conversionFactor}{
}

McCAD::Conversion::MCNPWriter::~MCNPWriter(){
}

void
McCAD::Conversion::MCNPWriter::operator()(
        const std::vector<std::shared_ptr<Geometry::Impl::Compound>>& compoundList,
        const std::shared_ptr<VoidCell>& voidCell){
    processSolids(compoundList);
    addUniqueSurfNumbers(compoundList);
    createMaterialsMap(inputConfig.materialsInfo);
    createSolidsMap(compoundList);
    createVoidMap(voidCell);
    // Create output file stream and write cells, surfaces, and data cards.
    if(std::filesystem::exists(inputConfig.MCOutputFileName)){
        std::string oldFileName{"old_" + inputConfig.MCOutputFileName};
        std::rename(inputConfig.MCOutputFileName.c_str(), oldFileName.c_str());
    }
    if(std::filesystem::exists(inputConfig.volumesFileName)){
        std::string oldFileName{"old_" + inputConfig.volumesFileName};
        std::rename(inputConfig.volumesFileName.c_str(), oldFileName.c_str());
    }
    std::ofstream outputStream(inputConfig.MCOutputFileName.c_str()),
                  volumeStream(inputConfig.volumesFileName.c_str());
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
        const std::vector<std::shared_ptr<Geometry::Impl::Compound>>& compoundList){
    TaskQueue<Policy::Parallel> taskQueue;
    for(const auto& compound : compoundList){
        taskQueue.submit([this, compound](){
            if(compound->cylSolidsList.size() > 0){
                // Generate assist conversion surfaces.
                for(const auto& cylSolidObj : compound->cylSolidsList){
                    Decomposition::AssistSurfaceGenerator{inputConfig}(*cylSolidObj);
                }
            }
            for(const auto& solidObj : compound->solidsList){
                MCNPExprGenerator{inputConfig.precision, scalingFactor}(solidObj);
            }
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
            //Standard_Boolean equalParmts{Standard_True};
            std::vector<Standard_Boolean> equalParmts;
            for (Standard_Integer i = 0; i < surface->accessSImpl()->surfParameters.size(); ++i){
                if (std::abs(surface->accessSImpl()->surfParameters[i] -
                              member.second->accessSImpl()->surfParameters[i]) > inputConfig.precision){
                    equalParmts.push_back(Standard_False);
                } else equalParmts.push_back(Standard_True);
            }
            if(surface->accessSImpl()->surfSymb == "GQ"){
                // Check if only the last parameter in the cylinder coefficients is different.
                if (std::none_of(equalParmts.cbegin(), equalParmts.cend() - 1, std::logical_not<bool>()) &&
                        equalParmts.back() == Standard_False){
                    if (std::abs(surface->accessSImpl()->surfParameters.back() -
                                  member.second->accessSImpl()->surfParameters.back()) < 1.0e-2)
                        equalParmts.back() = Standard_True;
                }
            }
            if (std::none_of(equalParmts.cbegin(), equalParmts.cend(), std::logical_not<bool>()))
                return member.first;
        }
    }
    return std::nullopt;
}

void
McCAD::Conversion::MCNPWriter::addUniqueSurfNumbers(
        const std::vector<std::shared_ptr<Geometry::Impl::Compound>>& compoundList){
    Standard_Integer surfNumber = inputConfig.startSurfNum;
    for(const auto& compound: compoundList){
        for(const auto& solidObj : compound->solidsList){
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
                } else if(surface->accessSImpl()->surfaceType == Tools::toTypeName(GeomAbs_Torus)){
                    auto duplicateID = findDuplicate(surface, uniqueTori);
                    if(duplicateID){
                        surface->accessSImpl()->uniqueID = *duplicateID;
                    } else{
                        surface->accessSImpl()->uniqueID = surfNumber;
                        uniqueTori[surfNumber] = surface;
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
                } else if(surface->accessSImpl()->surfaceType == Tools::toTypeName(GeomAbs_Torus)){
                    auto duplicateID = findDuplicate(surface, uniqueTori);
                    if(duplicateID){
                        surface->accessSImpl()->uniqueID = *duplicateID;
                    } else{
                        surface->accessSImpl()->uniqueID = surfNumber;
                        uniqueTori[surfNumber] = surface;
                        uniqueSurfaces[surfNumber] = surface->accessSImpl()->surfExpr;
                        ++surfNumber;
                    }
                }
            }
        }
    }
}

void
McCAD::Conversion::MCNPWriter::createSolidsMap(
        const std::vector<std::shared_ptr<Geometry::Impl::Compound>>& compoundList){
    for(const auto& compound : compoundList){
        compound->matID = materialsMap[compound->matInfo];
        compoundObjMap[compound->compoundID] = compound;
        for(const auto& solidObj : compound->solidsList){
            solidObjMap[solidObj->accessSImpl()->solidID] = solidObj;
        }
    }
}

void
McCAD::Conversion::MCNPWriter::createMaterialsMap(
        const std::vector<std::tuple<std::string, double>>& materialsInfo){
    Standard_Integer counter = inputConfig.startMatNum;
    for(const auto& mat : materialsInfo){
        if(materialsMap.find(mat) == materialsMap.end()){
            materialsMap[mat] = counter;
            ++counter;
        }
    }
}

void
McCAD::Conversion::MCNPWriter::addDaughterVoids(const std::shared_ptr<VoidCell>& voidCell){
    if(inputConfig.BVHVoid){
        // Add all inner and leaf nodes in the binary tree.
        if(voidCell->daughterVoidCells.size() > 0){
            // Add daughter void cells.
            for(const auto& daughter : voidCell->daughterVoidCells){
                std::tuple<Standard_Integer, Standard_Integer, std::string> voidDesignator =
                        std::make_tuple(daughter->depth, daughter->width, voidCell->key);
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
            std::tuple<Standard_Integer, Standard_Integer, std::string> voidDesignator =
                    std::make_tuple(voidCell->depth, voidCell->width, voidCell->key);
            voidCellsMap[voidDesignator] = voidCell;
        }
    }
}

void
McCAD::Conversion::MCNPWriter::createVoidMap(
        const std::shared_ptr<VoidCell>& voidCell){
    // Add root void cell, also it is the graveyard.
    std::tuple<Standard_Integer, Standard_Integer, std::string> voidDesignator =
            std::make_tuple(voidCell->depth, voidCell->width, voidCell->key);
    voidCellsMap[voidDesignator] = voidCell;
    if(inputConfig.voidGeneration) addDaughterVoids(voidCell);
    Standard_Integer voidSurfNumber = inputConfig.startSurfNum + uniqueSurfaces.size();
    for (const auto& member : voidCellsMap){
        MCNPExprGenerator{inputConfig.precision, scalingFactor}(member.second);
        member.second->voidSurfNumber = voidSurfNumber;
        uniqueSurfaces[voidSurfNumber] = member.second->voidSurfExpr;
        ++voidSurfNumber;
    }
}

std::string
McCAD::Conversion::MCNPWriter::adjustLineWidth(const std::string& mainExpr,
                                               const std::string& bodyExpr,
                                               Standard_Integer& continueSpacing){
    // Adjust cell solids expression to the chosen maximum number of columns.
    std::string finalExpr = mainExpr;
    std::vector<std::string> splitExpr;
    boost::split(splitExpr, bodyExpr, [](char c) {return c == ' ';});
    Standard_Integer lineIndex{1};
    for(Standard_Integer i = 0; i < splitExpr.size(); ++i){
        if((finalExpr.size() + splitExpr[i].size() + 1) > inputConfig.maxLineWidth*lineIndex){
            finalExpr.resize(inputConfig.maxLineWidth*lineIndex, *const_cast<char*>(" "));
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
McCAD::Conversion::MCNPWriter::writeHeader(std::ofstream& outputStream){
    Standard_Integer materialCells{0};
    if(inputConfig.componentIsSingleCell) materialCells = compoundObjMap.size();
    else materialCells = solidObjMap.size();
    auto timeStart{std::chrono::system_clock::now()};
    std::time_t timeStart_t = std::chrono::system_clock::to_time_t(timeStart);
    outputStream << boost::str(boost::format("McCAD v%s generated %s input file. ")
                               % McCAD::Info::getMcCADVersion()
                               % inputConfig.MCcode) <<
                    std::ctime(&timeStart_t) <<
                    "C     * Material Cells ---- " << materialCells <<
                    "\nC     * Surfaces       ---- " << uniqueSurfaces.size() <<
                    "\nC     * Void cells     ---- " << voidCellsMap.size() << std::endl;
}

void
McCAD::Conversion::MCNPWriter::writeCellCard(std::ofstream& outputStream,
                                             std::ofstream& volumeStream){
    outputStream << "c ==================== Cell Cards ====================" << std::endl;
    Standard_Integer cellNumber = inputConfig.startCellNum;
    // Need to loop over all solids in a compSolid, write header with maerial,
    // component name, cell range, etc. Adjust width of expression
    for(const auto& compound : compoundObjMap){
        outputStream << "c ============" <<
                        "\nc * Component: " << compound.second->compoundName <<
                        "\nc * Subsolids: " << compound.second->solidsList.size() <<
                        "\nc * Material : " << std::get<0>(compound.second->matInfo) <<
                        "\nc * Density  : " << std::get<1>(compound.second->matInfo);
        Standard_Integer numCells{1};
        if(!inputConfig.componentIsSingleCell) numCells = compound.second->solidsList.size();
        outputStream << "\nc * Cells    : " << cellNumber << " - " << cellNumber + numCells - 1 <<
                        "\nc ============" << std::endl;
        if(inputConfig.componentIsSingleCell){
            std::string cellExpr{boost::str(boost::format("%d") % cellNumber)};
            if (cellExpr.size() < 5) cellExpr.resize(5, *const_cast<char*>(" "));
            continueSpacing = cellExpr.size() + 1;
            // Add materials.
            if(std::get<0>(compound.second->matInfo) == "void" ||
                    std::get<1>(compound.second->matInfo) == 0.0){
                cellExpr += boost::str(boost::format(" %9.4f") % 0.0);
            } else{
                cellExpr += boost::str(boost::format(" %d %9.5f")
                                       % compound.second->matID
                                       % std::get<1>(compound.second->matInfo));
            }
            std::string cellSolidsExpr;
            Standard_Real compoundVolume{0};
            for(Standard_Integer i = 0; i < compound.second->solidsList.size(); ++i){
                if(i == 0) cellSolidsExpr += " (";
                else cellSolidsExpr += " : (";
                MCNPExprGenerator{}.genCellExpr(compound.second->solidsList.at(i));
                cellSolidsExpr += compound.second->solidsList.at(i)->accessSImpl()->cellExpr;
                cellSolidsExpr += ")";
                compoundVolume += compound.second->solidsList.at(i)->accessSImpl()->solidVolume;
            }
            cellSolidsExpr += " Imp:N=1.0 Imp:P=1.0 Imp:E=0.0";
            outputStream << adjustLineWidth(cellExpr, cellSolidsExpr, continueSpacing) << std::endl;
            std::string compoundData{boost::str(boost::format("%d  %11.5f  %s")
                                                 % cellNumber
                                                 % (compoundVolume*std::pow(scalingFactor, 3))
                                                 % compound.second->compoundName)};
            volumeStream << compoundData << std::endl;
            ++cellNumber;
        } else {
            Standard_Real solidVolume{0};
            for(Standard_Integer i = 0; i < compound.second->solidsList.size(); ++i){
                std::string cellExpr{boost::str(boost::format("%d") % cellNumber)};
                if (cellExpr.size() < 5) cellExpr.resize(5, *const_cast<char*>(" "));
                continueSpacing = cellExpr.size() + 1;
                // Add materials.
                if(std::get<0>(compound.second->matInfo) == "void" ||
                        std::get<1>(compound.second->matInfo) == 0.0){
                    cellExpr += boost::str(boost::format(" %9.4f") % 0.0);
                } else{
                    cellExpr += boost::str(boost::format(" %d %9.4f")
                                           % compound.second->matID
                                           % std::get<1>(compound.second->matInfo));
                }
                std::string cellSolidsExpr{" "};
                solidVolume = compound.second->solidsList.at(i)->accessSImpl()->solidVolume;
                MCNPExprGenerator{}.genCellExpr(compound.second->solidsList.at(i));
                cellSolidsExpr += compound.second->solidsList.at(i)->accessSImpl()->cellExpr;
                cellSolidsExpr += " Imp:N=1.0 Imp:P=1.0 Imp:E=0.0";
                outputStream << adjustLineWidth(cellExpr, cellSolidsExpr, continueSpacing) << std::endl;
                std::string solidData{boost::str(boost::format("%d %11.5f %s")
                                                    % cellNumber
                                                    % (solidVolume*std::pow(scalingFactor, 3))
                                                    % compound.second->compoundName)};
                volumeStream << solidData << std::endl;
                ++cellNumber;
            }
        }
    }
}

void
McCAD::Conversion::MCNPWriter::writeVoidCard(std::ofstream& outputStream){
    Standard_Integer voidNumber{inputConfig.startCellNum};
    if(inputConfig.componentIsSingleCell) voidNumber += compoundObjMap.size();
    else voidNumber += solidObjMap.size();
    outputStream << "c ==================== Void Cells ====================" << std::endl;
    if(!inputConfig.voidGeneration) {
        // Generate a single void cell.
        std::string voidExpr{boost::str(boost::format("%d") % voidNumber)};
        if (voidExpr.size() < 5) voidExpr.resize(5, *const_cast<char*>(" "));
        continueSpacing = voidExpr.size() + 1;
        voidExpr += boost::str(boost::format(" %d") % 0);
        std::string voidSolidsExpr;
        Standard_Integer voidSurfNumber{voidCellsMap[std::make_tuple(0,0,"r")]->voidSurfNumber};
        voidSolidsExpr += boost::str(boost::format(" %d") % (-1 * voidSurfNumber));
        Standard_Integer index = inputConfig.startCellNum;
        for(Standard_Integer i = 0; i < compoundObjMap.size(); ++i){
            voidSolidsExpr += boost::str(boost::format(" #%d") %index);
            ++index;
        }
        voidSolidsExpr += " Imp:N=1.0 Imp:P=1.0 Imp:E=0.0";
        outputStream << adjustLineWidth(voidExpr, voidSolidsExpr, continueSpacing) << std::endl;
        ++voidNumber;
        goto writeGraveYard;
    }
    for(const auto& member : voidCellsMap){
        std::string voidExpr{boost::str(boost::format("%d") % voidNumber)};
        if (voidExpr.size() < 5) voidExpr.resize(5, *const_cast<char*>(" "));
        continueSpacing = voidExpr.size() + 1;
        voidExpr += boost::str(boost::format(" %d") % 0);
        std::string voidSolidsExpr;
        voidSolidsExpr += boost::str(boost::format(" %d") % (-1 * member.second->voidSurfNumber));
        if(inputConfig.BVHVoid){
            if(member.second->daughterVoidCells.size() > 0){
                // Write inner nodes.
                for(const auto& daughterVoid : member.second->daughterVoidCells){
                    voidSolidsExpr += boost::str(boost::format(" %d")
                                                 % voidCellsMap[std::make_tuple(
                                daughterVoid->depth, daughterVoid->width, daughterVoid->key)]->voidSurfNumber);
                }
            } else{
                // Write leaf node. Write complement of solids in the void cell.
                for(const Standard_Integer& solidID : member.second->solidIDList){
                    voidSolidsExpr += solidObjMap[solidID]->accessSImpl()->complimentExpr;
                }
            }
        } else{
            // Only ignore the root cell is th map size is > 1. In some cases a
            // single void cell is generated hich in turn would be the root one.
            if(member.first == std::make_tuple(0,0,"r") && voidCellsMap.size() > 1) continue;
            // Write leaf nodes only.
            for(const Standard_Integer& solidID : member.second->solidIDList){
                voidSolidsExpr += solidObjMap[solidID]->accessSImpl()->complimentExpr;
            }
        }
        voidSolidsExpr += " Imp:N=1.0 Imp:P=1.0 Imp:E=0.0";
        outputStream << adjustLineWidth(voidExpr, voidSolidsExpr, continueSpacing) << std::endl;
        ++voidNumber;
    }
    writeGraveYard:;
    outputStream << "c ==================== Graveyard ====================" << std::endl;
    Standard_Integer voidSurfNumber{voidCellsMap[std::make_tuple(0,0,"r")]->voidSurfNumber};
    std::string graveYardExpr{boost::str(boost::format("%d") % voidNumber)};
    if (graveYardExpr.size() < 5) graveYardExpr.resize(5, *const_cast<char*>(" "));
    continueSpacing = graveYardExpr.size() + 1;
    graveYardExpr += boost::str(boost::format(" %d") % 0);
    graveYardExpr += boost::str(boost::format(" %d") % voidSurfNumber);
    graveYardExpr += " Imp:N=0.0 Imp:P=0.0 Imp:E=0.0 $Graveyard";
    outputStream << graveYardExpr << std::endl;
    uniqueSurfaces[voidSurfNumber] = voidCellsMap[std::make_tuple(0,0,"r")]->voidSurfExpr;
    // Add cell to calculate volumes.
    outputStream << "c ========== Start of volume calculation parameters ==========" << std::endl;
    std::string volumeCellExpr{boost::str(boost::format("%d") % (voidNumber + 1))};
    if (volumeCellExpr.size() < 5) volumeCellExpr.resize(5, *const_cast<char*>(" "));
    continueSpacing = volumeCellExpr.size() + 1;
    volumeCellExpr += boost::str(boost::format(" %d") % 0);
    volumeCellExpr += boost::str(boost::format(" -%d %d Imp:N=1.0 Imp:P=1.0 Imp:E=0.0")
                                 % (uniqueSurfaces.size() + inputConfig.startSurfNum)
                                 % voidSurfNumber);
    outputStream << "c " << volumeCellExpr << std::endl;
    std::string volumeCellGYExpr{boost::str(boost::format("%d") % (voidNumber + 2))};
    if (volumeCellGYExpr.size() < 5) volumeCellGYExpr.resize(5, *const_cast<char*>(" "));
    continueSpacing = volumeCellGYExpr.size() + 1;
    volumeCellGYExpr += boost::str(boost::format(" %d") % 0);
    volumeCellGYExpr += boost::str(boost::format(" %d Imp:N=0.0 Imp:P=0.0 Imp:E=0.0")
                                 % (uniqueSurfaces.size() + inputConfig.startSurfNum));
    outputStream << "c " << volumeCellGYExpr << std::endl;
    outputStream << "c ========== End of volume calculation parameters ==========" << std::endl;
}

void
McCAD::Conversion::MCNPWriter::writeSurfCard(std::ofstream& outputStream){
    outputStream << "\nc ==================== Surface Cards ====================" << std::endl;
    // Write all surfaces.
    for(const auto& surface : uniqueSurfaces){
        std::string surfExpr{boost::str(boost::format("%d") % surface.first)};
        if (surfExpr.size() < 5) surfExpr.resize(5, *const_cast<char*>(" "));
        surfExpr += " ";
        continueSpacing = surfExpr.size() + 1;
        outputStream << adjustLineWidth(surfExpr, surface.second, continueSpacing) << std::endl;
    }
    // Add spherical surface to calculate the volumes.
    outputStream << "c ========== Start of volume calculation parameters ==========" << std::endl;
    Standard_Real xExt, yExt, zExt;
    auto& graveyard = voidCellsMap[std::make_tuple(0,0,"r")];
    xExt = std::get<2>(graveyard->xAxis) - std::get<0>(graveyard->xAxis);
    yExt = std::get<2>(graveyard->yAxis) - std::get<0>(graveyard->yAxis);
    zExt = std::get<2>(graveyard->zAxis) - std::get<0>(graveyard->zAxis);
    radius = std::max(std::max(xExt, yExt), zExt)*scalingFactor;
    std::string surfExpr{boost::str(boost::format("%d")
                                    % (inputConfig.startSurfNum + uniqueSurfaces.size()))};
    if (surfExpr.size() < 5) surfExpr.resize(5, *const_cast<char*>(" "));
    surfExpr += " ";
    continueSpacing = surfExpr.size() + 1;
    std::string surfCoord{boost::str(boost::format("S %6.3f %6.3f %6.3f %6.3f")
                            % (std::get<1>(graveyard->xAxis)*scalingFactor)
                            % (std::get<1>(graveyard->yAxis)*scalingFactor)
                            % (std::get<1>(graveyard->zAxis)*scalingFactor)
                            % (radius))};
    outputStream << "c " << adjustLineWidth(surfExpr, surfCoord, continueSpacing) << std::endl;
    outputStream << "c ========== End of volume calculation parameters ==========" << std::endl;
}

void
McCAD::Conversion::MCNPWriter::writeDataCard(std::ofstream& outputStream){
    outputStream << "\nc ==================== Data Cards ====================\n";
    // Write Materials:
    for(const auto& mat : materialsMap){
        if(std::get<0>(mat.first) == "void") continue;
        outputStream << "c ============" <<
                        "\nc * Material : " << std::get<0>(mat.first) <<
                        "\nc * Density  : " << std::get<1>(mat.first) <<
                        "\nc ============" <<
                        "\nM" << mat.second << std::endl;
    }
    // add tallies and source to calculate volumes.
    outputStream << "Mode N" << "\nNPS 5e8" << "\nPRDMP 1e7 1e7 j 1 j" << std::endl;
    outputStream << "c ========== Start of volume calculation parameters ==========" << std::endl;
    std::string sourceExpr{boost::str(
                    boost::format("c Void \nc SDEF ERG=14.1 SUR=%d PAR=1 NRM=-1 WGT=%6.3f $Pi*r^2")
                    % (inputConfig.startSurfNum + uniqueSurfaces.size())
                    % (inputConfig.PI * std::pow(radius, 2)))};
    outputStream << sourceExpr << std::endl;
    if(compoundObjMap.size() > 1){
        Standard_Integer numCells{1};
        if(inputConfig.componentIsSingleCell) numCells = compoundObjMap.size();
        else numCells = solidObjMap.size();
        std::string volTallyExpr{boost::str(boost::format("F4:N %d %di %d")
                                            % inputConfig.startCellNum
                                            % (numCells - 2)
                                            % (inputConfig.startCellNum + numCells - 1))};
        volTallyExpr += boost::str(boost::format("\nc SD4 1 %dr") % (numCells - 1));
        outputStream << "c " << volTallyExpr << std::endl;
    } else {
        std::string volTallyExpr{boost::str(boost::format("F4:N %d")
                                            % inputConfig.startCellNum)};
        volTallyExpr += boost::str(boost::format("\nc SD4 1"));
        outputStream << "c " << volTallyExpr << std::endl;
    }
    outputStream << "c ========== End of volume calculation parameters ==========" << std::endl;
}

