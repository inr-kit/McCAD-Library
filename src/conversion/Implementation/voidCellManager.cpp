// McCAD
#include "voidCellManager.hpp"
#include "TaskQueue.hpp"
#include "splitSurfaceSelector.hpp"

McCAD::Conversion::VoidCellManager::VoidCellManager(const IO::InputConfig& inputConfig) :
    BVHVoid{inputConfig.BVHVoid}, minVoidVolume{inputConfig.minVoidVolume},
    maxSolidsPerVoidCell{inputConfig.maxSolidsPerVoidCell},
    voidGeneration{ inputConfig.voidGeneration }, debugLevel{ inputConfig.debugLevel }{
}

McCAD::Conversion::VoidCellManager::VoidCellManager(const bool& BVHVoid,
                                                    const double& minVoidVolume,
                                                    const int& maxSolidsPerVoidCell,
                                                    const bool& voidGeneration,
                                                    const int& debugLevel) :
    BVHVoid{BVHVoid}, minVoidVolume{minVoidVolume}, maxSolidsPerVoidCell{maxSolidsPerVoidCell},
    voidGeneration{voidGeneration}, debugLevel{ debugLevel }{
}

McCAD::Conversion::VoidCellManager::~VoidCellManager(){}

/** ********************************************************************
* @brief   Creates the root void cell.
* @detail  The operator is used to create the root void cell which includes all solids. 
*          If voidGeneration is false, the root cell is used to define the graveyard.
* @param   solidObjList: a vector of pointers to loaded solids objects.
* @returns voidCell: a shared pointer to the created root void cell.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
std::shared_ptr<McCAD::Conversion::VoidCell>
McCAD::Conversion::VoidCellManager::operator()(
        const McCAD::Conversion::VoidCellManager::solidsList& solidObjList){
    if (voidGeneration) std::cout << "   - Generating void" << std::endl;
    else                std::cout << "   - Creating graveyard" << std::endl;
    // Create the root void cell, the outside of which is the graveyard.
    voidCell = std::make_shared<VoidCell>();
    membersMap members = createLists(solidObjList);
    perform(members);
    return voidCell;
}

/** ********************************************************************
* @brief   Creates a void cell.
* @detail  The operator is used to create a void cell using a map of solids.
* @param   members: map of solids contained in the void cell.
* @param   depth: the depth of the void cell in the binary tree.
* @param   width: the width of the void cell in the binary tree, 0 means left and 1 means right.
* @param   key: string identifies whether the void cell is the left or right half.
* @returns voidCell: a shared pointer to the created void cell.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
std::shared_ptr<McCAD::Conversion::VoidCell>
McCAD::Conversion::VoidCellManager::operator()(
        const McCAD::Conversion::VoidCellManager::membersMap& members,
        const int& depth, const int& width, const std::string& key){
    // Operator used for BVH void cells.
    voidCell = std::make_shared<VoidCell>(depth, width, key);
    perform(members);
    return voidCell;
}

/** ********************************************************************
* @brief   Creates a void cell.
* @detail  The operator is used to create a void cell using a map of solids.
* @param   members: map of solids contained in the void cell.
* @param   depth: the depth of the void cell in the binary tree.
* @param   width: the width of the void cell in the binary tree, 0 means left and 1 means right.
* @param   key: string identifies whether the void cell is the left or right half.
* @param   xAxisAABB, yAxisAABB, zAxisAABB: updated extent of the void cell.
* @returns voidCell: a shared pointer to the created void cell.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
std::shared_ptr<McCAD::Conversion::VoidCell>
McCAD::Conversion::VoidCellManager::operator()(
        const McCAD::Conversion::VoidCellManager::membersMap& members,
        const int& depth, const int& width,
        const std::string& key, const aabbTuple& xAxisAABB,
        const aabbTuple& yAxisAABB, const aabbTuple& zAxisAABB){
    voidCell = std::make_shared<VoidCell>(depth, width, key);
    voidCell->xAxisUpdate = xAxisAABB;
    voidCell->yAxisUpdate = yAxisAABB;
    voidCell->zAxisUpdate = zAxisAABB;
    perform(members);
    return voidCell;
}

/** ********************************************************************
* @brief   Creates a map with solids AABB as values and the solids IDs as keys.
* @param   solidObjList: a vector of pointers to loaded solids objects.
* @returns members: the created maps of solids.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
McCAD::Conversion::VoidCellManager::membersMap
McCAD::Conversion::VoidCellManager::createLists(
        const McCAD::Conversion::VoidCellManager::solidsList& solidObjList){
    membersMap members;
    for(const auto& solidObj : solidObjList){
        members[solidObj->accessSImpl()->solidID] =
                std::make_tuple(solidObj->accessSImpl()->aabb, "NA", 0.0);
    }
    return members;
}

/** ********************************************************************
* @brief   The main function that performs void generation.
* @detail  The function operates on the list of solids and selects and splits voide
*          cells recursively.
* @param   members: a map of solids.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::VoidCellManager::perform(
        const McCAD::Conversion::VoidCellManager::membersMap& members){
    // empty void cells are allowed in the current conversion algorithm!
    if (members.size() > 0){
        populateLists(members);
        updateVoidCell(members);
    }
    bool splitCondition = false;
    if (voidGeneration){
        // for depths > 1 you have to carry the dims of the half parent downstream to
        // update the final resultant void cells.
        if(!BVHVoid) voidCell->updateAABB();
        bool splitConditionOnNumSolids = members.size() > maxSolidsPerVoidCell
                ? true : false;
        bool splitConditionOnCellVol = voidCell->getAABBVolume() >= 2 * minVoidVolume
                ? true : false;
        splitCondition = splitConditionOnNumSolids && splitConditionOnCellVol;
    }
    // Debug level >= 1
    if (debugLevel >= 1) {
        voidCell->outputAABB();
    }
    if(splitCondition){
        auto surface = SplitSurfaceSelector{maxSolidsPerVoidCell, minVoidVolume
                                           }.process(xAxis, yAxis, zAxis, voidCell);
        if (!surface) return;
        auto splitMembers = splitVoidCell(*surface, members);
        voidCell->splitted = true;
        if(BVHVoid){
            // Create left void cell.
            auto voidCellLeft = VoidCellManager{BVHVoid, minVoidVolume,
                    maxSolidsPerVoidCell, voidGeneration, debugLevel}(
                        splitMembers.first, voidCell->depth + 1, 0, voidCell->key + "L");
            // Add the created left void cell as daughter.
            voidCell->daughterVoidCells.push_back(voidCellLeft);
            // Create right void cell.
            auto voidCellRight = VoidCellManager{BVHVoid, minVoidVolume,
                    maxSolidsPerVoidCell, voidGeneration, debugLevel}(
                        splitMembers.second, voidCell->depth + 1, 1, voidCell->key + "R");
            // Add the created right void cell as daughter.
            voidCell->daughterVoidCells.push_back(voidCellRight);
        } else{
            // Create left void cell.
            auto leftBoundaries = updateBoundaries(*surface, 0);
            auto voidCellLeft = VoidCellManager{BVHVoid, minVoidVolume,
                    maxSolidsPerVoidCell, voidGeneration, debugLevel}(
                        splitMembers.first, voidCell->depth + 1, 0, voidCell->key + "L",
                        leftBoundaries[0], leftBoundaries[1], leftBoundaries[2]);
            // Add the created left void cell as daughter.
            voidCell->daughterVoidCells.push_back(voidCellLeft);
            // Create right void cell.
            auto rightBoundaries = updateBoundaries(*surface, 1);
            auto voidCellRight = VoidCellManager{BVHVoid, minVoidVolume,
                    maxSolidsPerVoidCell, voidGeneration, debugLevel}(
                        splitMembers.second, voidCell->depth + 1, 1, voidCell->key + "R",
                        rightBoundaries[0], rightBoundaries[1], rightBoundaries[2]);
            // Add the created right void cell as daughter.
            voidCell->daughterVoidCells.push_back(voidCellRight);
        }
    }
}

/** ********************************************************************
* @brief   The function creates list of AABB extension along the 3 axises.
* @param   members: a map of solids.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::VoidCellManager::populateLists(
        const McCAD::Conversion::VoidCellManager::membersMap& members){
    double minX{0}, minY{0}, minZ{0}, maxX{0}, maxY{0}, maxZ{0};
    for(const auto& member : members){
        std::get<0>(member.second).Get(minX, minY, minZ, maxX, maxY, maxZ);
        xAxis[member.first] = std::make_tuple(minX, minX + std::abs(maxX-minX)/2.0, maxX);
        yAxis[member.first] = std::make_tuple(minY, minY + std::abs(maxY-minY)/2.0, maxY);
        zAxis[member.first] = std::make_tuple(minZ, minZ + std::abs(maxZ-minZ)/2.0, maxZ);
    }
}

/** ********************************************************************
* @brief   The function adds solids and solids IDs to the void cell object.
* @param   members: a map of solids.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
void
McCAD::Conversion::VoidCellManager::updateVoidCell(
        const McCAD::Conversion::VoidCellManager::membersMap& members){
    voidCell->addSolidIDs(members);
    voidCell->addSolids(members);
}

/** ********************************************************************
* @brief   The main function that performs splitting of the void cell.
* @param   members: a map of solids in the void cell.
* @param   surface: the selected splitting surface.
* @returns pair of solid maps.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
std::pair<McCAD::Conversion::VoidCellManager::membersMap,
          McCAD::Conversion::VoidCellManager::membersMap>
McCAD::Conversion::VoidCellManager::splitVoidCell(
        const McCAD::Conversion::VoidCellManager::surfaceTuple& surface,
        const McCAD::Conversion::VoidCellManager::membersMap& members){
    if (std::get<0>(surface) == "X"){
        return splitMembersList(surface, members, xAxis);
    } else if(std::get<0>(surface) == "Y"){
        return splitMembersList(surface, members, yAxis);
    } else if(std::get<0>(surface) == "Z"){
        return splitMembersList(surface, members, zAxis);
    } else
        throw std::runtime_error("The chosen splitting surface is of an unknown type!.\n"
                                 "This is possibly caused by a code error, please report!.");
}

/** ********************************************************************
* @brief   The main function that performs splitting of the solids map.
* @param   surface: the selected splitting surface.
* @param   members: a map of solids in the void cell.
* @param   axis: a map of the extent of the solids AABBS along a specifid axis.
* @returns A pair of solid maps.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
std::pair<McCAD::Conversion::VoidCellManager::membersMap,
          McCAD::Conversion::VoidCellManager::membersMap>
McCAD::Conversion::VoidCellManager::splitMembersList(
        const McCAD::Conversion::VoidCellManager::surfaceTuple& surface,
        const McCAD::Conversion::VoidCellManager::membersMap& members,
        const McCAD::Conversion::VoidCellManager::dimMap& axis){
    membersMap membersLeft, membersRight;
    std::string surfaceType = std::get<0>(surface);
    double surfaceCoord = std::get<1>(surface);
    for(const auto& member : members){
        auto ID = member.first;
        auto dimensions = axis.at(ID);
        if(std::get<0>(dimensions) >= surfaceCoord){
            // Add to solids on the right.
            membersRight[ID] = std::make_tuple(std::get<0>(member.second), "NA", 0.0);
        } else if(std::get<2>(dimensions) <= surfaceCoord){
            // Add to solids on the left.
            membersLeft[ID] = std::make_tuple(std::get<0>(member.second), "NA", 0.0);
        } else {
            // Solid overlaps, add to both lists.
            // update aabb first by cutting then add to list.
            auto AABBLeft = updateOverlapAABB(std::get<0>(member.second), surface, 0);
            membersLeft[ID] = std::make_tuple(AABBLeft, surfaceType, surfaceCoord);
            auto AABBRight = updateOverlapAABB(std::get<0>(member.second), surface, 1);
            membersRight[ID] = std::make_tuple(AABBRight, surfaceType, surfaceCoord);
        }
    }
    return std::make_pair(membersLeft, membersRight);
}

/** ********************************************************************
* @brief   The function updates the extent of the AABB of the solid.
* @detail  The function updates the coordinates of the void cell after splitting.
*          The updated coordinates are then saved in the void cell object.
* @param   surface: the selected splitting surface.
* @param   sense: the position of the updated AABB relative to the splitting surface.
* @returns Updated AABB coordinates of the void cell.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
McCAD::Conversion::VoidCellManager::aabbVec
McCAD::Conversion::VoidCellManager::updateBoundaries(
        McCAD::Conversion::VoidCellManager::surfaceTuple& surface,
        const int& sense){
    aabbTuple xAxisAABB, yAxisAABB, zAxisAABB;
    std::string surfaceType = std::get<0>(surface);
    double surfaceCoord = std::get<1>(surface);
    if (surfaceType == "X"){
        yAxisAABB = std::make_tuple(voidCell->minY, voidCell->maxY);
        zAxisAABB = std::make_tuple(voidCell->minZ, voidCell->maxZ);
        if(sense == 0) {
            xAxisAABB = std::make_tuple(voidCell->minX, surfaceCoord);
        } else{
            xAxisAABB = std::make_tuple(surfaceCoord, voidCell->maxX);
        }
    } else if(surfaceType == "Y"){
        xAxisAABB = std::make_tuple(voidCell->minX, voidCell->maxX);
        zAxisAABB = std::make_tuple(voidCell->minZ, voidCell->maxZ);
        if(sense == 0) {
            yAxisAABB = std::make_tuple(voidCell->minY, surfaceCoord);
        } else{
            yAxisAABB = std::make_tuple(surfaceCoord, voidCell->maxY);
        }
    } else if(surfaceType == "Z"){
        xAxisAABB = std::make_tuple(voidCell->minX, voidCell->maxX);
        yAxisAABB = std::make_tuple(voidCell->minY, voidCell->maxY);
        if(sense == 0) {
            zAxisAABB = std::make_tuple(voidCell->minZ, surfaceCoord);
        } else{
            zAxisAABB = std::make_tuple(surfaceCoord, voidCell->maxZ);
        }
    } else
        throw std::runtime_error("The chosen splitting surface is of an unknown type!.\n"
                                 "This is possibly caused by a code error, please report!.");
     aabbVec xyzAABB{xAxisAABB, yAxisAABB, zAxisAABB};
     return xyzAABB;
}

/** ********************************************************************
* @brief   The function updates the extent of the AABB of the solid.
* @detail  The function shifts the boundary of the AABB to locate it either to the right or left
*          of the splitting surface. It only changes the AABB stored in the map to be used for 
*          subsequent splittings of the void cells. It doesn't change the original AABB in the
*          solid object itself.
* @param   aAABB: the AABB of the solid.
* @param   surface:the selected splitting surface.
* @param   sense: the desired position of the updated AABB relative to the splitting surface. 
* @returns An updated AABB.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
Bnd_Box
McCAD::Conversion::VoidCellManager::updateOverlapAABB(
        const Bnd_Box& aAABB,
        const McCAD::Conversion::VoidCellManager::surfaceTuple& surface,
        const int& sense){
    double surfaceCoord = std::get<1>(surface);
    double minX{0}, minY{0}, minZ{0}, maxX{0}, maxY{0}, maxZ{0};
    aAABB.Get(minX, minY, minZ, maxX, maxY, maxZ);
    // 0 sense means the desired final AABB is to the left and 1 is to the right of the surface.
    if (std::get<0>(surface) == "X"){
        if(sense == 0) maxX = surfaceCoord;
        else           minX = surfaceCoord;
    } else if(std::get<0>(surface) == "Y"){
        if(sense == 0) maxY = surfaceCoord;
        else           minY = surfaceCoord;
    } else{
        if(sense == 0) maxZ = surfaceCoord;
        else           minZ = surfaceCoord;
    }
    Bnd_Box newAABB;
    gp_Pnt minPoint(minX, minY, minZ);
    gp_Pnt maxPoint(maxX, maxY, maxZ);
    newAABB.Add(minPoint);
    newAABB.Add(maxPoint);
    return newAABB;
}
