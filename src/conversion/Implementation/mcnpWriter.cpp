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

/** ********************************************************************
* @brief   The operator calls the specialized writer functions of cell, surface, and data cards. 
* @param   compoundList is a list of compound objects.
* @param   voidCell is a pointer to the root void cell.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPWriter::operator()(
        const std::vector<std::shared_ptr<Geometry::Impl::Compound>>& compoundList,
        const std::shared_ptr<VoidCell>& voidCell){
    processSolids(compoundList);
    addUniqueSurfNumbers(compoundList);
    materialSurfacesCount = uniqueSurfaces.size();
    createMaterialsMap(inputConfig.materialsInfo);
    createSolidsMap(compoundList);
    createVoidMap(voidCell);
    // Create output file stream and write cells, surfaces, and data cards.
    if(std::filesystem::exists(inputConfig.MCFileName)){
        std::string oldFileName{"old_" + inputConfig.MCFileName};
        std::rename(inputConfig.MCFileName.c_str(), oldFileName.c_str());
    }
    if(std::filesystem::exists(inputConfig.volumesFileName)){
        std::string oldFileName{"old_" + inputConfig.volumesFileName};
        std::rename(inputConfig.volumesFileName.c_str(), oldFileName.c_str());
    }
    if (std::filesystem::exists(inputConfig.voidCellsFileName)) {
        std::string oldFileName{ "old_" + inputConfig.voidCellsFileName };
        std::rename(inputConfig.voidCellsFileName.c_str(), oldFileName.c_str());
    }
    std::ofstream outputStream(inputConfig.MCFileName.c_str()),
                  volumeStream(inputConfig.volumesFileName.c_str()),
                  voidCellsStream(inputConfig.voidCellsFileName.c_str());
    writeHeaders(outputStream, volumeStream, voidCellsStream);
    writeCellCard(outputStream, volumeStream);
    writeVoidCard(outputStream, voidCellsStream);
    writeSurfCard(outputStream);
    writeDataCard(outputStream);
    outputStream.close();
    volumeStream.close();
    voidCellsStream.close();
}

/** ********************************************************************
* @brief   The operator calls the proper cell expression generator of solids.
* @param   compoundList is a list of compound objects.
* @date    31/12/2021
* modified 14/11/2022
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPWriter::processSolids(
        const std::vector<std::shared_ptr<Geometry::Impl::Compound>>& compoundList){
    TaskQueue<Policy::Parallel> taskQueue;
    for(const auto& compound : compoundList){
        taskQueue.submit([this, compound](){
            // If the solid contains cylinders, generate assisting surfaces.
            if(compound->cylSolidsList.size() > 0){
                for(const auto& cylSolidObj : compound->cylSolidsList){
                    Decomposition::AssistSurfaceGenerator{inputConfig}(*cylSolidObj);
                }
            }
            // If the solid contains cones, generate assisting surfaces.
            if (compound->conSolidsList.size() > 0) {
                for (const auto& conSolidObj : compound->conSolidsList) {
                    Decomposition::AssistSurfaceGenerator{ inputConfig }(*conSolidObj);
                }
            }
            // Generate MCNP expression for the solids in the compound object.
            for(const auto& solidObj : compound->solidsList){
                MCNPExprGenerator{inputConfig.precision, scalingFactor, inputConfig.angularTolerance}(solidObj);
            }
        });
    }
    taskQueue.complete();
}

/** ********************************************************************
* @brief    The function checks if the surface already exists in the unique surfaces list.
* @param    surface is a surface to check for in the unique list of surfaces.
* @param    uniqueMap is a map of unique surfaces IDs and surfaces.
* @returns  An optional integer ID of the duplicate surface.
* @date     31/12/2021
* @modified 03/11/2022
* @author   Moataz Harb
* **********************************************************************/
std::optional<int>
McCAD::Conversion::MCNPWriter::findDuplicate(
        const std::shared_ptr<Geometry::BoundSurface>& surface,
        McCAD::Conversion::MCNPWriter::surfacesMap& uniqueMap){
    // Compare both surface sybmol and parameters.
    if(uniqueMap.size() == 0) return std::nullopt;
    for (const auto& member : uniqueMap){
        if (surface->accessSImpl()->surfSymb == member.second->accessSImpl()->surfSymb){
            // compare parameters.
            std::vector<bool> equalParmts;
            for (int i = 0; i < surface->accessSImpl()->surfParameters.size(); ++i){
                if (std::abs(surface->accessSImpl()->surfParameters[i] -
                              member.second->accessSImpl()->surfParameters[i]) >= inputConfig.precision){
                    equalParmts.push_back(false);
                } else equalParmts.push_back(true);
            }
            if(surface->accessSImpl()->surfSymb == "GQ"){
                // Some GQs result from tori simplification. For pipes with fillings, the difference in some
                // coefficients is neglegible.
                // Check if any of the cylinder coefficients is different.
                for (int i = 0; i < equalParmts.size(); ++i) {
                    if (equalParmts[i] == false) {
                        if (std::abs(surface->accessSImpl()->surfParameters[i] -
                            member.second->accessSImpl()->surfParameters[i]) < 1.0e-1 &&
                            std::abs((surface->accessSImpl()->surfParameters[i] -
                                member.second->accessSImpl()->surfParameters[i]) /
                                surface->accessSImpl()->surfParameters[i]) < inputConfig.precision)
                            equalParmts[i] = true;
                    }
                }
            }
            if (std::none_of(equalParmts.cbegin(), equalParmts.cend(), std::logical_not<bool>()))
                return member.first;
        }
    }
    return std::nullopt;
}

/** ********************************************************************
* @brief    The function creates a unique ID for all surfaces of all solids in the list of compounds.
* @param    compoundList is a list of compound objects.
* @date     31/12/2021
* @modified 03/11/2022
* @author   Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPWriter::addUniqueSurfNumbers(
        const std::vector<std::shared_ptr<Geometry::Impl::Compound>>& compoundList){
    int surfNumber = inputConfig.startSurfNum;
    for(const auto& compound: compoundList){
        for(const auto& solidObj : compound->solidsList){
            for(const auto& surface : solidObj->accessSImpl()->intersectionList){
                if(surface->accessSImpl()->surfaceType == Tools::toTypeName(GeomAbs_Plane)){
                    // Surface is plane. Compare to unique planes list and create a unique ID.
                    auto duplicateID = findDuplicate(surface, uniquePlanes);
                    if(duplicateID){
                        // An ID of duplicate surface is returned.
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
                } else if (surface->accessSImpl()->surfaceType == Tools::toTypeName(GeomAbs_Cone)) {
                    auto duplicateID = findDuplicate(surface, uniqueCones);
                    if (duplicateID) {
                        surface->accessSImpl()->uniqueID = *duplicateID;
                    }
                    else {
                        surface->accessSImpl()->uniqueID = surfNumber;
                        uniqueCones[surfNumber] = surface;
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
                } else if (surface->accessSImpl()->surfaceType == Tools::toTypeName(GeomAbs_Cone)) {
                    auto duplicateID = findDuplicate(surface, uniqueCones);
                    if (duplicateID) {
                        surface->accessSImpl()->uniqueID = *duplicateID;
                    }
                    else {
                        surface->accessSImpl()->uniqueID = surfNumber;
                        uniqueCones[surfNumber] = surface;
                        uniqueSurfaces[surfNumber] = surface->accessSImpl()->surfExpr;
                        ++surfNumber;
                    }
                }
            }
        }
    }
}

/** ********************************************************************
* @brief   The function creates a map of solids.
* @param   compoundList is a list of compounds.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
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

/** ********************************************************************
* @brief   The function creates a map of material info.
* @param   materialsInfo is a list of materials names and densities.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPWriter::createMaterialsMap(
        const std::vector<std::tuple<std::string, double>>& materialsInfo){
    int matNumber = inputConfig.startMatNum;
    for(const auto& mat : materialsInfo){
        if(materialsMap.find(mat) == materialsMap.end()){
            materialsMap[mat] = matNumber;
            ++matNumber;
        }
    }
}

/** ********************************************************************
* @brief   The function adds daughter void cells to the void map.
* @param   voidCell is the root void cell.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPWriter::addDaughterVoids(const std::shared_ptr<VoidCell>& voidCell){
    if(inputConfig.BVHVoid){
        // Add all inner and leaf nodes in the binary tree.
        if(voidCell->daughterVoidCells.size() > 0){
            // Add daughter void cells.
            for(const auto& daughter : voidCell->daughterVoidCells){
                std::tuple<int, int, std::string> voidDesignator =
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
            std::tuple<int, int, std::string> voidDesignator =
                    std::make_tuple(voidCell->depth, voidCell->width, voidCell->key);
            voidCellsMap[voidDesignator] = voidCell;
        }
    }
}

/** ********************************************************************
* @brief   The function creates a map of void cells.
* @param   voidCell is the root void cell.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPWriter::createVoidMap(
        const std::shared_ptr<VoidCell>& voidCell){
    // Add root void cell, also it is the graveyard.
    std::tuple<int, int, std::string> voidDesignator =
            std::make_tuple(voidCell->depth, voidCell->width, voidCell->key);
    voidCellsMap[voidDesignator] = voidCell;
    if(inputConfig.voidGeneration) addDaughterVoids(voidCell);
    int voidSurfNumber = inputConfig.startSurfNum + uniqueSurfaces.size();
    for (const auto& member : voidCellsMap){
        MCNPExprGenerator{inputConfig.precision, scalingFactor, inputConfig.angularTolerance}(member.second);
        member.second->voidSurfNumber = voidSurfNumber;
        uniqueSurfaces[voidSurfNumber] = member.second->voidSurfExpr;
        ++voidSurfNumber;
    }
}

/** ********************************************************************
* @brief   The function adjusts the cell expression to a 80 columns maximum per line.
* @param   mainExpr is a string of cell ID and material density.
* @param   bodyExpr is a string of surface expressions.
* @param   continueSpacing is the number of white spaces to prepend on the continue line.
* @returns Adjected string.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
std::string
McCAD::Conversion::MCNPWriter::adjustLineWidth(const std::string& mainExpr,
                                               const std::string& bodyExpr,
                                               int& continueSpacing){
    // Adjust cell solids expression to the chosen maximum number of columns.
    std::string finalExpr = mainExpr;
    std::vector<std::string> splitExpr;
    boost::split(splitExpr, bodyExpr, [](char c) {return c == ' ';});
    int lineIndex{1};
    for(int i = 0; i < splitExpr.size(); ++i){
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

/** ********************************************************************
* @brief    The function writes the header of the MC file.
* @param    outputStream is a string stream to write to.
* @date     31/12/2021
* @modified 09/06/2022
* @author   Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPWriter::writeHeaders(std::ofstream& outputStream, 
                                           std::ofstream& volumeStream, 
                                           std::ofstream& voidCellsStream){
    // Write MC file header.
    int materialCells{0};
    if(inputConfig.compoundIsSingleCell) materialCells = compoundObjMap.size();
    else materialCells = solidObjMap.size();
    auto timeStart{std::chrono::system_clock::now()};
    std::time_t timeStart_t = std::chrono::system_clock::to_time_t(timeStart);
    outputStream << boost::str(boost::format("McCAD v%s generated %s input file / ")
                               % McCAD::Info::McCADVersion
                               % inputConfig.MCcode) <<
                    std::ctime(&timeStart_t) <<
                    "C   * Material Cells    ---- " << materialCells <<
                    "\nC   * Material Surfaces ---- " << materialSurfacesCount <<
                    "\nC   * Void cells        ---- " << voidCellsMap.size() << 
                    "\nC   * Void Surfaces     ---- " << uniqueSurfaces.size() - materialSurfacesCount << std::endl;
    // Write volumes file header.
    volumeStream << boost::str(boost::format("McCAD v%s generated volumes file / ")
                               % McCAD::Info::McCADVersion) << std::ctime(&timeStart_t) << 
                    boost::str(boost::format("File contents: a map of cell IDs to CAD volume and name of the corresponding solid.")) <<
                    boost::str(boost::format("\nColumn 1 is the cell ID, column 2 is the volume [cubic %s], and column 3 is the compound name.")
                                             % inputConfig.units) << std::endl;
    // Write voidCells file header.
    voidCellsStream << boost::str(boost::format("McCAD v%s generated void-to-material cell IDs mapping file / ")
                                  % McCAD::Info::McCADVersion) << std::ctime(&timeStart_t) <<
                       boost::str(boost::format("File contents: a map of void cell IDs to the material cell IDs contained within.")) <<
                       "\nColumn 1 is the void cell ID and column(s) 2(+) is(are) the material cell ID(s)." << std::endl;
}

/** ********************************************************************
* @brief   The function writes the cell cards to the MC file and writes a map of cell ID and volume info to volumes file.
* @param   outputStream is a string stream to write cell expressions to.
* @param   volumeStream is a string stream to write volume and cell IDs to.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPWriter::writeCellCard(std::ofstream& outputStream,
                                             std::ofstream& volumeStream){
    outputStream << "c ================================= Cell Cards =================================" << std::endl;
    int cellNumber = inputConfig.startCellNum;
    // Need to loop over all solids in a compSolid, write header with material,
    // component name, cell range, etc. Adjust width of expression to 80 columns.
    for(const auto& compound : compoundObjMap){
        outputStream << "c ============" <<
                        "\nc * Compound : " << compound.second->compoundName <<
                        "\nc * Subsolids: " << compound.second->solidsList.size() <<
                        "\nc * Material : " << std::get<0>(compound.second->matInfo) <<
                        "\nc * Density  : " << std::get<1>(compound.second->matInfo);
        int numCells{1};
        if(!inputConfig.compoundIsSingleCell) numCells = compound.second->solidsList.size();
        outputStream << "\nc * Cells    : " << cellNumber << " - " << cellNumber + numCells - 1 <<
                        "\nc ============" << std::endl;
        if(inputConfig.compoundIsSingleCell){
            // Add cell ID to compound object
            compound.second->MCCellID = cellNumber;
            std::string cellExpr{boost::str(boost::format("%d") % cellNumber)};
            if (cellExpr.size() < 5) cellExpr.resize(5, *const_cast<char*>(" "));
            continueSpacing = cellExpr.size() + 1;
            // Add materials.
            if(std::get<0>(compound.second->matInfo) == "void" ||
                    std::get<1>(compound.second->matInfo) == 0.0){
                cellExpr += boost::str(boost::format(" %d") % 0.0);
            } else{
                cellExpr += boost::str(boost::format(" %d %9.5f")
                                       % compound.second->matID
                                       % std::get<1>(compound.second->matInfo));
            }
            std::string cellSolidsExpr;
            double compoundVolume{0};
            for(int i = 0; i < compound.second->solidsList.size(); ++i){
                if(i == 0) cellSolidsExpr += " (";
                else cellSolidsExpr += " : (";
                MCNPExprGenerator{}.genCellExpr(compound.second->solidsList.at(i));
                cellSolidsExpr += compound.second->solidsList.at(i)->accessSImpl()->cellExpr;
                cellSolidsExpr += ")";
                compoundVolume += compound.second->solidsList.at(i)->accessSImpl()->solidVolume;
            }
            cellSolidsExpr += " Imp:N=1.0 Imp:P=1.0 Imp:E=0.0 TMP=2.53E-8 $U=100000";
            outputStream << adjustLineWidth(cellExpr, cellSolidsExpr, continueSpacing) << std::endl;
            // Write compound volume data to the string stream.
            std::string compoundData{ boost::str(boost::format("%d") % cellNumber) };
            if (compoundData.size() < 5) compoundData.resize(5, *const_cast<char*>(" "));
            compoundData += boost::str(boost::format(" %9.5E %s") % (compoundVolume*std::pow(scalingFactor, 3))
                                                                  % compound.second->compoundName);
            volumeStream << compoundData << std::endl;
            ++cellNumber;
        } else {
            double solidVolume{0};
            for(int i = 0; i < compound.second->solidsList.size(); ++i){
                // Add cell ID to solid object.
                compound.second->solidsList.at(i)->accessSImpl()->MCCellID = cellNumber;
                std::string cellExpr{boost::str(boost::format("%d") % cellNumber)};
                if (cellExpr.size() < 5) cellExpr.resize(5, *const_cast<char*>(" "));
                continueSpacing = cellExpr.size() + 1;
                // Add materials.
                if(std::get<0>(compound.second->matInfo) == "void" ||
                        std::get<1>(compound.second->matInfo) == 0.0){
                    cellExpr += boost::str(boost::format(" %d") % 0.0);
                } else{
                    cellExpr += boost::str(boost::format(" %d %9.5f")
                                           % compound.second->matID
                                           % std::get<1>(compound.second->matInfo));
                }
                std::string cellSolidsExpr{" "};
                solidVolume = compound.second->solidsList.at(i)->accessSImpl()->solidVolume;
                MCNPExprGenerator{}.genCellExpr(compound.second->solidsList.at(i));
                cellSolidsExpr += compound.second->solidsList.at(i)->accessSImpl()->cellExpr;
                cellSolidsExpr += " Imp:N=1.0 Imp:P=1.0 Imp:E=0.0 TMP=2.53E-8 $U=100000";
                outputStream << adjustLineWidth(cellExpr, cellSolidsExpr, continueSpacing) << std::endl;
                std::string solidData{boost::str(boost::format("%d %9.5E %s")
                                                    % cellNumber
                                                    % (solidVolume*std::pow(scalingFactor, 3))
                                                    % compound.second->compoundName)};
                volumeStream << solidData << std::endl;
                ++cellNumber;
            }
        }
    }
}

/** ********************************************************************
* @brief    The function writes voids cell cards to the MC file and writes a map of void cell ID and cell ID to thre voidCells file.
* @param    outputStream is a string stream to write cell expressions to.
* @param    voidCellsStream is a string stream to write void-cell ID mapping to.
* @date     31/12/2021
* @modified 13/12/2022
* @author   Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPWriter::writeVoidCard(std::ofstream& outputStream,
                                             std::ofstream& voidCellsStream){
    int voidNumber{inputConfig.startCellNum};
    if(inputConfig.compoundIsSingleCell) voidNumber += compoundObjMap.size();
    else voidNumber += solidObjMap.size();
    outputStream << "c ================================= Void Cells =================================" << std::endl;
    if(!inputConfig.voidGeneration) {
        // Add void cell ID to voidCell object.
        voidCellsMap[std::make_tuple(0, 0, "r")]->MCCellID = voidNumber;
        // Generate a single void cell.
        std::string voidExpr{boost::str(boost::format("%d") % voidNumber)};
        if (voidExpr.size() < 5) voidExpr.resize(5, *const_cast<char*>(" "));
        continueSpacing = voidExpr.size() + 1;
        voidExpr += boost::str(boost::format(" %d") % 0);
        std::string voidSolidsExpr;
        int voidSurfNumber{voidCellsMap[std::make_tuple(0, 0, "r")]->voidSurfNumber};
        voidSolidsExpr += boost::str(boost::format(" %d") % (-1 * voidSurfNumber));
        std::string voidCellMapData{ boost::str(boost::format("%d") % voidNumber) };
        if (voidCellMapData.size() < 5) voidCellMapData.resize(5, *const_cast<char*>(" "));
        for(const auto& compound : compoundObjMap){
            if (inputConfig.compoundIsSingleCell) {
                // Add compound cell ID.
                voidSolidsExpr += boost::str(boost::format(" #%d") % compound.second->MCCellID);
                voidCellMapData += boost::str(boost::format(" %d") % compound.second->MCCellID);
            }
            else {
                // Loop over all solids in a compound to add their cell IDs.
                for (int i = 0; i < compound.second->solidsList.size(); ++i) {
                    voidSolidsExpr += boost::str(boost::format(" #%d") % compound.second->solidsList.at(i)->accessSImpl()->MCCellID);
                    voidCellMapData += boost::str(boost::format(" %d") % compound.second->solidsList.at(i)->accessSImpl()->MCCellID);
                }
            }
        }
        voidSolidsExpr += " Imp:N=1.0 Imp:P=1.0 Imp:E=0.0 TMP=2.53E-8 $U=100000";
        outputStream << adjustLineWidth(voidExpr, voidSolidsExpr, continueSpacing) << std::endl;
        // Write void cell - material cells map to the string stream.
        voidCellsStream << voidCellMapData << std::endl;
        ++voidNumber;
        goto writeGraveYard;
    }
    for(const auto& member : voidCellsMap){
        // Add void cell ID to voidCell object.
        member.second->MCCellID = voidNumber;
        std::string voidExpr{boost::str(boost::format("%d") % voidNumber)};
        if (voidExpr.size() < 5) voidExpr.resize(5, *const_cast<char*>(" "));
        continueSpacing = voidExpr.size() + 1;
        voidExpr += boost::str(boost::format(" %d") % 0.0);
        std::string voidSolidsExpr;
        voidSolidsExpr += boost::str(boost::format(" %d") % (-1 * member.second->voidSurfNumber));
        // Initialise void cell - material cells map string stream.
        std::string voidCellMapData{ boost::str(boost::format("%d") % voidNumber) };
        if (voidCellMapData.size() < 5) voidCellMapData.resize(5, *const_cast<char*>(" "));
        if(inputConfig.BVHVoid){
            if(member.second->daughterVoidCells.size() > 0){
                // Write inner nodes.
                for(const auto& daughterVoid : member.second->daughterVoidCells){
                    voidSolidsExpr += boost::str(boost::format(" %d")
                                                 % daughterVoid->voidSurfNumber);
                }
            } else{
                // Write leaf nodes. Write complement of solids in the void cell.
                int ID;
                for(const int& solidID : member.second->solidIDList){
                    voidSolidsExpr += solidObjMap[solidID]->accessSImpl()->complimentExpr;
                    if (!inputConfig.compoundIsSingleCell)
                        voidCellMapData += boost::str(boost::format(" %d") % solidObjMap[solidID]->accessSImpl()->MCCellID);
                    else {
                        if (solidObjMap[solidID]->accessSImpl()->compoundID != ID) {
                            ID = solidObjMap[solidID]->accessSImpl()->compoundID;
                            voidCellMapData += boost::str(boost::format(" %d") % compoundObjMap[ID]->MCCellID);
                        }
                    }
                }
            }
        } else{
            // Only ignore the root cell if the map size is > 1. In some cases a
            // single void cell is generated which in turn would be the root one.
            if(member.first == std::make_tuple(0, 0, "r") && voidCellsMap.size() > 1) continue;
            // Write leaf nodes only.
            int ID;
            for(const int& solidID : member.second->solidIDList){
                voidSolidsExpr += solidObjMap[solidID]->accessSImpl()->complimentExpr;
                if (!inputConfig.compoundIsSingleCell)
                    voidCellMapData += boost::str(boost::format(" %d") % solidObjMap[solidID]->accessSImpl()->MCCellID);
                else {
                    if (solidObjMap[solidID]->accessSImpl()->compoundID != ID) {
                        ID = solidObjMap[solidID]->accessSImpl()->compoundID;
                        voidCellMapData += boost::str(boost::format(" %d") % compoundObjMap[ID]->MCCellID);
                    }
                }
            }
        }
        voidSolidsExpr += " Imp:N=1.0 Imp:P=1.0 Imp:E=0.0 TMP=2.53E-8 $U=100000";
        outputStream << adjustLineWidth(voidExpr, voidSolidsExpr, continueSpacing) << std::endl;
        // Write void cell - material cells map to the string stream.
        voidCellsStream << voidCellMapData << std::endl;
        ++voidNumber;
    }
    writeGraveYard:;
    outputStream << "c ================================= Graveyard ==================================" << std::endl;
    int voidSurfNumber{voidCellsMap[std::make_tuple(0, 0, "r")]->voidSurfNumber};
    std::string graveYardExpr{boost::str(boost::format("%d") % voidNumber)};
    if (graveYardExpr.size() < 5) graveYardExpr.resize(5, *const_cast<char*>(" "));
    continueSpacing = graveYardExpr.size() + 1;
    graveYardExpr += boost::str(boost::format(" %d") % 0.0);
    graveYardExpr += boost::str(boost::format(" %d") % voidSurfNumber);
    graveYardExpr += " Imp:N=0.0 Imp:P=0.0 Imp:E=0.0 TMP=2.53E-8 $U=100000";
    outputStream << graveYardExpr << std::endl;
    //uniqueSurfaces[voidSurfNumber] = voidCellsMap[std::make_tuple(0, 0, "r")]->voidSurfExpr;
    // Add cell to calculate volumes.
    outputStream << "c ==================== Start of volumes calculation cells ======================" <<
                    "\nc To start volume calculations comment out the grave yard cell above and uncomment the two cells below" << std::endl;
    std::string volumeCellExpr{boost::str(boost::format("%d") % (voidNumber + 1))};
    if (volumeCellExpr.size() < 5) volumeCellExpr.resize(5, *const_cast<char*>(" "));
    continueSpacing = volumeCellExpr.size() + 1;
    volumeCellExpr += boost::str(boost::format(" %d") % 0.0);
    volumeCellExpr += boost::str(boost::format(" -%d %d Imp:N=1.0 Imp:P=1.0 Imp:E=0.0 TMP=2.53E-8 $U=100000")
                                 % (uniqueSurfaces.size() + inputConfig.startSurfNum)
                                 % voidSurfNumber);
    outputStream << "c " << volumeCellExpr << std::endl;
    std::string volumeCellGYExpr{boost::str(boost::format("%d") % (voidNumber + 2))};
    if (volumeCellGYExpr.size() < 5) volumeCellGYExpr.resize(5, *const_cast<char*>(" "));
    continueSpacing = volumeCellGYExpr.size() + 1;
    volumeCellGYExpr += boost::str(boost::format(" %d") % 0.0);
    volumeCellGYExpr += boost::str(boost::format(" %d Imp:N=0.0 Imp:P=0.0 Imp:E=0.0 TMP=2.53E-8 $U=100000")
                                 % (uniqueSurfaces.size() + inputConfig.startSurfNum));
    outputStream << "c " << volumeCellGYExpr << std::endl;
    outputStream << "c ====================== End of volumes calculation cells ======================" << std::endl;
}

/** ********************************************************************
* @brief   The function writes surface cards to the MC file.
* @param   outputStream is a string stream to write cell expressions to.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPWriter::writeSurfCard(std::ofstream& outputStream){
    outputStream << "\nc ================================ Surface Cards ===============================" << std::endl;
    // Write all surfaces.
    for(const auto& surface : uniqueSurfaces){
        std::string surfExpr{boost::str(boost::format("%d") % surface.first)};
        if (surfExpr.size() < 5) surfExpr.resize(5, *const_cast<char*>(" "));
        surfExpr += " ";
        continueSpacing = surfExpr.size() + 1;
        outputStream << adjustLineWidth(surfExpr, surface.second, continueSpacing) << std::endl;
    }
    // Add a spherical surface to calculate cell volumes.
    outputStream << "c =================== Start of volumes calculation surfaces ====================" << std::endl;
    double xExt, yExt, zExt;
    auto& graveyard = voidCellsMap[std::make_tuple(0, 0, "r")];
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
    outputStream << "c ==================== End of volumes calculation surfaces =====================" << std::endl;
}

/** ********************************************************************
* @brief   The function writes data cards to the MC file.
* @param   outputStream is a string stream to write cell expressions to.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::MCNPWriter::writeDataCard(std::ofstream& outputStream){
    outputStream << "\nc ================================= Data Cards =================================\n" <<
                    "Mode N" << "\nNPS 5e8" << "\nPRDMP 1e7 1e7 j 1 j" << "\nLost 10 10" << std::endl;
    // Write Materials.
    for(const auto& mat : materialsMap){
        if(std::get<0>(mat.first) == "void") continue;
        outputStream << "c ============" <<
                        "\nc * Material : " << std::get<0>(mat.first) <<
                        "\nc * Density  : " << std::get<1>(mat.first) <<
                        "\nc ============" <<
                        "\nc M" << mat.second << std::endl;
    }
    // add tallies and source to calculate volumes.
    outputStream << "c =================== Start of volume calculation parameters ===================" << std::endl;
    std::string sourceExpr{boost::str(
                    boost::format("c Void \nc SDEF ERG=14.1 SUR=%d PAR=1 NRM=-1 WGT=%9.5f $Pi*r^2")
                    % (inputConfig.startSurfNum + uniqueSurfaces.size())
                    % (inputConfig.PI * std::pow(radius, 2)))};
    outputStream << sourceExpr << std::endl;
    if(compoundObjMap.size() > 1){
        int numCells{1};
        if(inputConfig.compoundIsSingleCell) numCells = compoundObjMap.size();
        else numCells = solidObjMap.size();
        std::string volTallyExpr{boost::str(boost::format("F4:N %d %di %d t")
                                            % inputConfig.startCellNum
                                            % (numCells - 2)
                                            % (inputConfig.startCellNum + numCells - 1))};
        volTallyExpr += boost::str(boost::format("\nc SD4 1 %dr") % (numCells));
        outputStream << "c " << volTallyExpr << std::endl;
    } else {
        std::string volTallyExpr{boost::str(boost::format("F4:N %d")
                                            % inputConfig.startCellNum)};
        volTallyExpr += boost::str(boost::format("\nc SD4 1"));
        outputStream << "c " << volTallyExpr << std::endl;
    }
    outputStream << "c ==================== End of volume calculation parameters ====================" << std::endl;
}

