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
#include "mcxWriter.hpp"
#include "TaskQueue.hpp"
#include "mcnpExpressionGenerator.hpp"
#include "SurfaceUtilities.hpp"
#include "AssistSurfaceGenerator.hpp"
//
#include "stdafx.hpp"
#include "tinystr.hpp"
#include "tinyxml.hpp"

McCAD::Conversion::MCXWriter::MCXWriter(const IO::InputConfig& inputConfig) :
    inputConfig{inputConfig}, scalingFactor{1.0/inputConfig.conversionFactor}{
}

McCAD::Conversion::MCXWriter::~MCXWriter(){
}

/** ********************************************************************
* @brief   The operator calls the specialized writer functions of NECP-MCX input cards. 
* @param   compoundList is a list of compound objects.
* @param   voidCell is a pointer to the root void cell.
* @date    15/11/2022
* @author  Jiandi Guo
* **********************************************************************/
void
McCAD::Conversion::MCXWriter::operator()(
        const std::vector<std::shared_ptr<Geometry::Impl::Compound>>& compoundList,
        const std::shared_ptr<VoidCell>& voidCell){
     processSolids(compoundList);
     addUniqueSurfNumbers(compoundList);
     materialSurfacesCount = uniqueSurfaces.size();
     createMaterialsMap(inputConfig.materialsInfo);
     createSolidsMap(compoundList);
     createVoidMap(voidCell);
    // Create output file stream and write cells, surfaces, and data cards.

    /*if(std::filesystem::exists(inputConfig.MCFileName)){
        std::string oldFileName{"old_" + inputConfig.MCFileName};
        std::rename(inputConfig.MCFileName.c_str(), oldFileName.c_str());
    }*/
    TiXmlDocument* pDocument = new TiXmlDocument(inputConfig.MCFileName.c_str());
    if (NULL == pDocument)
    {
        return;
    }
    
    TiXmlElement* pRoot = new TiXmlElement("input");
    pDocument->LinkEndChild(pRoot);

    WriteGeometry(pRoot);
    WriteMaterials(pRoot);
    WriteOptions(pRoot);
    WriteSources(pRoot);
    WriteTallies(pRoot);
    WritePlots(pRoot);
    //save mcx input file
    pDocument->SaveFile(inputConfig.MCFileName.c_str());
}

/** ********************************************************************
* @brief   The operator calls the proper cell expression generator of solids.
* @param   compoundList is a list of compound objects.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
 void
 McCAD::Conversion::MCXWriter::processSolids(
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
             // Generate MCNP expression for the solids in the compound object.
             for(const auto& solidObj : compound->solidsList){
                 MCNPExprGenerator{inputConfig.precision, inputConfig.angularTolerance,scalingFactor}(solidObj);
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
* @modified 02/06/2022
* @author   Moataz Harb
* **********************************************************************/
 std::optional<int>
 McCAD::Conversion::MCXWriter::findDuplicate(
         const std::shared_ptr<Geometry::BoundSurface>& surface,
         McCAD::Conversion::MCXWriter::surfacesMap& uniqueMap){
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
* @brief   The function creates a unique ID for all surfaces of all solids in the list of compounds.
* @param   compoundList is a list of compound objects.
* @date    31/12/2021
* @author  Moataz Harb
* **********************************************************************/
 void
 McCAD::Conversion::MCXWriter::addUniqueSurfNumbers(
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
                         uniqueMcxSurfaces[surfNumber] = surface->accessSImpl()->surfMcxPar;
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
                         uniqueMcxSurfaces[surfNumber] = surface->accessSImpl()->surfMcxPar;
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
                         uniqueMcxSurfaces[surfNumber] = surface->accessSImpl()->surfMcxPar;
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
                         uniqueMcxSurfaces[surfNumber] = surface->accessSImpl()->surfMcxPar;
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
                         uniqueMcxSurfaces[surfNumber] = surface->accessSImpl()->surfMcxPar;
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
                         uniqueMcxSurfaces[surfNumber] = surface->accessSImpl()->surfMcxPar;
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
                         uniqueMcxSurfaces[surfNumber] = surface->accessSImpl()->surfMcxPar;
                         ++surfNumber;
                     }
                 } else if(surface->accessSImpl()->surfaceType == Tools::toTypeName(GeomAbs_Cone)) {
                     auto duplicateID = findDuplicate(surface, uniqueCones);
                     if (duplicateID) {
                         surface->accessSImpl()->uniqueID = *duplicateID;
                     }
                     else {
                         surface->accessSImpl()->uniqueID = surfNumber;
                         uniqueCones[surfNumber] = surface;
                         uniqueSurfaces[surfNumber] = surface->accessSImpl()->surfExpr;
                         uniqueMcxSurfaces[surfNumber] = surface->accessSImpl()->surfMcxPar;
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
 McCAD::Conversion::MCXWriter::createSolidsMap(
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
 McCAD::Conversion::MCXWriter::createMaterialsMap(
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
 McCAD::Conversion::MCXWriter::addDaughterVoids(const std::shared_ptr<VoidCell>& voidCell){
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
 McCAD::Conversion::MCXWriter::createVoidMap(
         const std::shared_ptr<VoidCell>& voidCell){
     // Add root void cell, also it is the graveyard.
     std::tuple<int, int, std::string> voidDesignator =
             std::make_tuple(voidCell->depth, voidCell->width, voidCell->key);
     voidCellsMap[voidDesignator] = voidCell;
     if(inputConfig.voidGeneration) addDaughterVoids(voidCell);
     int voidSurfNumber = inputConfig.startSurfNum + uniqueSurfaces.size();
     for (const auto& member : voidCellsMap){
         MCNPExprGenerator{inputConfig.precision, inputConfig.angularTolerance,scalingFactor}(member.second);
         member.second->voidSurfNumber = voidSurfNumber;
         uniqueSurfaces[voidSurfNumber] = member.second->voidSurfExpr;
         uniqueMcxSurfaces[voidSurfNumber] = member.second->voidParMap;
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
 McCAD::Conversion::MCXWriter::adjustLineWidth(const std::string& mainExpr,
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
* @brief    The function writes the geometry of the NECP-MCX input file.
* @param    Parent is TiXmlElement: root node.
* @date     15/11/2022
* @modified 15/11/2022
* @author   Jiandi Guo
* **********************************************************************/
 void McCAD::Conversion::MCXWriter::WriteGeometry(TiXmlElement* Parent) {
     TiXmlElement* pChildGeo = new TiXmlElement("geometry");
     Parent->LinkEndChild(pChildGeo);
     //generate surface card
     for (const auto& surface : uniqueMcxSurfaces) {
         TiXmlElement* pChildSur = new TiXmlElement("surface");
         pChildSur->SetAttribute("id", surface.first);
         pChildSur->SetAttribute("type", (std::get<0>(surface.second)).c_str());
         pChildSur->SetAttribute("coeffs", (std::get<1>(surface.second)).c_str());
         pChildSur->SetAttribute("boundary", (std::get<2>(surface.second)).c_str());
         pChildGeo->LinkEndChild(pChildSur);
     }
     //generate material cell card
     int cellNumber = inputConfig.startCellNum;
     // Need to loop over all solids in a compSolid, write header with material,
     // component name, cell range, etc. Adjust width of expression to 80 columns.
     for (const auto& compound : compoundObjMap) {
         int numCells{ 1 };
         if (!inputConfig.compoundIsSingleCell) numCells = compound.second->solidsList.size();
         if (inputConfig.compoundIsSingleCell) {
             // Add cell ID to compound object
             compound.second->MCCellID = cellNumber;
             TiXmlElement* pChildCel = new TiXmlElement("cell");
             pChildCel->SetAttribute("id", cellNumber);
             // To add materials.
             pChildCel->SetAttribute("material", "void");
             std::string zone;
             for (int i = 0; i < compound.second->solidsList.size(); ++i) {
                 if (i == 0) zone += " (";
                 else zone += " : (";
                 MCNPExprGenerator{}.genCellExpr(compound.second->solidsList.at(i));
                 zone += compound.second->solidsList.at(i)->accessSImpl()->cellMcxExpr;
                 zone += ")";
             }
             pChildCel->SetAttribute("zone", zone.c_str());
             pChildCel->SetAttribute("imp:n", "1");
             pChildCel->SetAttribute("imp:p", "1");
             pChildGeo->LinkEndChild(pChildCel);
             ++cellNumber;
         }
         else {
             for (int i = 0; i < compound.second->solidsList.size(); ++i) {
                 // Add cell ID to solid object.
                 compound.second->solidsList.at(i)->accessSImpl()->MCCellID = cellNumber;
                 TiXmlElement* pChildCel = new TiXmlElement("cell");
                 pChildCel->SetAttribute("id", cellNumber);
                 // To add materials.
                 pChildCel->SetAttribute("material", "void");
                 std::string zone;
                 MCNPExprGenerator{}.genCellExpr(compound.second->solidsList.at(i));
                 zone += compound.second->solidsList.at(i)->accessSImpl()->cellMcxExpr;
                 pChildCel->SetAttribute("zone", zone.c_str());
                 pChildCel->SetAttribute("imp:n", "1.0");
                 pChildCel->SetAttribute("imp:p", "1.0");
                 pChildGeo->LinkEndChild(pChildCel);
                 ++cellNumber;
             }
         }
     }
     //generate void cell card
     int voidNumber{ inputConfig.startCellNum };
     if (inputConfig.compoundIsSingleCell) voidNumber += compoundObjMap.size();
     else voidNumber += solidObjMap.size();
     if (!inputConfig.voidGeneration) {
         // Add void cell ID to voidCell object.
         voidCellsMap[std::make_tuple(0, 0, "r")]->MCCellID = voidNumber;
         // Generate a single void cell.
         TiXmlElement* pChildVoid = new TiXmlElement("cell");
         pChildVoid->SetAttribute("id", voidNumber);
         // To add materials.
         pChildVoid->SetAttribute("material", "void");
         std::string zone;
         int voidSurfNumber{ voidCellsMap[std::make_tuple(0, 0, "r")]->voidSurfNumber };
         zone += std::to_string(-1 * voidSurfNumber);
         pChildVoid->SetAttribute("zone", zone.c_str());
         pChildVoid->SetAttribute("imp:n", "1.0");
         pChildVoid->SetAttribute("imp:p", "1.0");
         pChildGeo->LinkEndChild(pChildVoid);
         ++voidNumber;
         goto writeGraveYard;
     }
     for (const auto& member : voidCellsMap) {
         // Add void cell ID to voidCell object.
         member.second->MCCellID = voidNumber;
         TiXmlElement* pChildVoid = new TiXmlElement("cell");
         pChildVoid->SetAttribute("id", voidNumber);
         // To add materials.
         pChildVoid->SetAttribute("material", "void");
         std::string zone;
         zone += std::to_string(-1 * member.second->voidSurfNumber);
         if (inputConfig.BVHVoid) {
             if (member.second->daughterVoidCells.size() > 0) {
                 // Write inner nodes.
                 for (const auto& daughterVoid : member.second->daughterVoidCells) {
                     zone += daughterVoid->voidSurfNumber;
                 }
             }
             else {
                 // Write leaf nodes. Write complement of solids in the void cell.
                 int ID;
                 for (const int& solidID : member.second->solidIDList) {
                     zone += solidObjMap[solidID]->accessSImpl()->complimentMcxExpr;
                     }
             }
         }
         else {
             // Only ignore the root cell if the map size is > 1. In some cases a
             // single void cell is generated which in turn would be the root one.
             if (member.first == std::make_tuple(0, 0, "r") && voidCellsMap.size() > 1) continue;
             // Write leaf nodes only.
             int ID;
             for (const int& solidID : member.second->solidIDList) {
                 zone += solidObjMap[solidID]->accessSImpl()->complimentMcxExpr;
             }
         }
         pChildVoid->SetAttribute("zone", zone.c_str());
         pChildVoid->SetAttribute("imp:n", "1.0");
         pChildVoid->SetAttribute("imp:p", "1.0");
         pChildGeo->LinkEndChild(pChildVoid);
         ++voidNumber;
     }
     writeGraveYard:;
     int voidSurfNumber{ voidCellsMap[std::make_tuple(0, 0, "r")]->voidSurfNumber };
     TiXmlElement* pChildGra = new TiXmlElement("cell");
     pChildGra->SetAttribute("id", voidNumber);
     std::string zone;
     zone += std::to_string(voidSurfNumber);
     pChildGra->SetAttribute("material", "void");
     pChildGra->SetAttribute("zone", zone.c_str());
     pChildGra->SetAttribute("imp:n", "0.0");
     pChildGra->SetAttribute("imp:p", "0.0");
     pChildGeo->LinkEndChild(pChildGra);
 }

/** ********************************************************************
* @brief    The function writes the materials of the NECP-MCX input file.
* @param    Parent is TiXmlElement: root node.
* @date     15/11/2022
* @modified 15/11/2022
* @author   Jiandi Guo
* **********************************************************************/
void
McCAD::Conversion::MCXWriter::WriteMaterials(TiXmlElement* Parent) {
    TiXmlElement* pChildMats = new TiXmlElement("materials");
    Parent->LinkEndChild(pChildMats);
    TiXmlElement* pChildLib = new TiXmlElement("library");
    pChildLib->SetAttribute("type", "ace");
    pChildLib->SetAttribute("path", "../ace_library/xsdir");
    pChildMats->LinkEndChild(pChildLib);

    TiXmlElement* pChildMat = new TiXmlElement("material");
    pChildMat->SetAttribute("id", "1");
    pChildMat->SetAttribute("temperature", "293.6");
    pChildMats->LinkEndChild(pChildMat);

    TiXmlElement* pChildNuc1 = new TiXmlElement("nuclide");
    pChildNuc1->SetAttribute("id", "1001.71c");
    pChildNuc1->SetAttribute("ao", "6.688000e-02");
    pChildMat->LinkEndChild(pChildNuc1);

    TiXmlElement* pChildNuc2 = new TiXmlElement("nuclide");
    pChildNuc2->SetAttribute("id", "8016.71c");
    pChildNuc2->SetAttribute("ao", "3.340000e-02");
    pChildMat->LinkEndChild(pChildNuc2);
}

/** ********************************************************************
* @brief    The function writes the options of the NECP-MCX input file.
* @param    Parent is TiXmlElement: root node.
* @date     15/11/2022
* @modified 15/11/2022
* @author   Jiandi Guo
* **********************************************************************/
void
McCAD::Conversion::MCXWriter::WriteOptions(TiXmlElement* Parent) {
    TiXmlElement* pChildOpt = new TiXmlElement("options");
    pChildOpt->SetAttribute("run_mode", "fixed-source");
    pChildOpt->SetAttribute("particle_type", "n");
    pChildOpt->SetAttribute("batch", "200");
    pChildOpt->SetAttribute("particles", "100000");
    Parent->LinkEndChild(pChildOpt);
}

/** ********************************************************************
* @brief    The function writes the sources of the NECP-MCX input file.
* @param    Parent is TiXmlElement: root node.
* @date     15/11/2022
* @modified 15/11/2022
* @author   Jiandi Guo
* **********************************************************************/
void
McCAD::Conversion::MCXWriter::WriteSources(TiXmlElement* Parent) {
    TiXmlElement* pChildSours = new TiXmlElement("sources");
    Parent->LinkEndChild(pChildSours);
    TiXmlElement* pChildSour = new TiXmlElement("source");
    pChildSour->SetAttribute("id", "1");
    pChildSour->SetAttribute("type", "neutron");
    pChildSour->SetAttribute("strength", "1.0");
    pChildSours->LinkEndChild(pChildSour);
    TiXmlElement* pChildSpa = new TiXmlElement("space");
    pChildSpa->SetAttribute("type", "point");
    pChildSpa->SetAttribute("bins", "0.0 0.0 0.0");
    pChildSpa->SetAttribute("distribution", "1");
    pChildSour->LinkEndChild(pChildSpa);
    TiXmlElement* pChildEne = new TiXmlElement("energy");
    pChildEne->SetAttribute("type", "point");
    pChildEne->SetAttribute("bins", "14");
    pChildEne->SetAttribute("distribution", "1");
    pChildSour->LinkEndChild(pChildEne);
    TiXmlElement* pChildAng = new TiXmlElement("angle");
    pChildAng->SetAttribute("type", "isotropic");
    pChildSour->LinkEndChild(pChildAng);
}

/** ********************************************************************
* @brief    The function writes the tallies of the NECP-MCX input file.
* @param    Parent is TiXmlElement: root node.
* @date     15/11/2022
* @modified 15/11/2022
* @author   Jiandi Guo
* **********************************************************************/
void
McCAD::Conversion::MCXWriter::WriteTallies(TiXmlElement* Parent) {
    TiXmlElement* pChildTals = new TiXmlElement("tallies");
    Parent->LinkEndChild(pChildTals);
    TiXmlElement* pChildFil = new TiXmlElement("filter");
    pChildFil->SetAttribute("id", "1");
    pChildFil->SetAttribute("type", "cell");
    pChildFil->SetAttribute("bins", "1");
    pChildTals->LinkEndChild(pChildFil);
    TiXmlElement* pChildTal = new TiXmlElement("tally");
    pChildTal->SetAttribute("id", "1");
    pChildTal->SetAttribute("estimator", "tracklength");
    pChildTal->SetAttribute("filters", "1");
    pChildTal->SetAttribute("scores", "flux");
    pChildTals->LinkEndChild(pChildTal);
}

/** ********************************************************************
* @brief    The function writes the plots of the NECP-MCX input file.
* @param    Parent is TiXmlElement: root node.
* @date     15/11/2022
* @modified 15/11/2022
* @author   Jiandi Guo
* **********************************************************************/
void
McCAD::Conversion::MCXWriter::WritePlots(TiXmlElement* Parent) {
    TiXmlElement* pChildPlos = new TiXmlElement("plots");
    Parent->LinkEndChild(pChildPlos);
    TiXmlElement* pChildPlo = new TiXmlElement("plot");
    pChildPlo->SetAttribute("id", "1");
    pChildPlo->SetAttribute("color_by", "cell");
    pChildPlo->SetAttribute("basis", "xy");
    pChildPlo->SetAttribute("origin", "0.0 0.0 0.0");
    pChildPlo->SetAttribute("width", "100 100");
    pChildPlo->SetAttribute("pixels", "1000 1000");
    pChildPlos->LinkEndChild(pChildPlo);
}



