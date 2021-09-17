// McCAD
#include "voidCellManager.hpp"
#include "TaskQueue.hpp"
#include "splitSurfaceSelector.hpp"

McCAD::Conversion::VoidCellManager::VoidCellManager(const IO::InputConfig& inputConfig) :
    BVHVoid{inputConfig.BVHVoid}, minVoidVolume{inputConfig.minVoidVolume},
    maxSolidsPerVoidCell{inputConfig.maxSolidsPerVoidCell},
    voidGeneration{inputConfig.voidGeneration}{
}

McCAD::Conversion::VoidCellManager::VoidCellManager(const Standard_Boolean& BVHVoid,
                                                    const Standard_Real& minVoidVolume,
                                                    const Standard_Integer& maxSolidsPerVoidCell,
                                                    const Standard_Boolean& voidGeneration) :
    BVHVoid{BVHVoid}, minVoidVolume{minVoidVolume}, maxSolidsPerVoidCell{maxSolidsPerVoidCell},
    voidGeneration{voidGeneration}{
}

McCAD::Conversion::VoidCellManager::~VoidCellManager(){}

std::shared_ptr<McCAD::Conversion::VoidCell>
McCAD::Conversion::VoidCellManager::operator()(
        const McCAD::Conversion::VoidCellManager::solidsList& solidObjList){
    if (voidGeneration) std::cout << "   - Generating void" << std::endl;
    else std::cout << "   - Creating graveyard" << std::endl;
    // Create root void cell, the outside of which is the graveyard.
    voidCell = std::make_shared<VoidCell>();
    membersMap members = createLists(solidObjList);
    perform(members);
    return voidCell;
}

std::shared_ptr<McCAD::Conversion::VoidCell>
McCAD::Conversion::VoidCellManager::operator()(
        const McCAD::Conversion::VoidCellManager::membersMap& members,
        const Standard_Integer& depth, const Standard_Integer& width,
        const std::string& key){
    // Operator used for BVH void cells.
    voidCell = std::make_shared<VoidCell>(depth, width, key);
    if (members.size() > 0) perform(members);
    return voidCell;
}

std::shared_ptr<McCAD::Conversion::VoidCell>
McCAD::Conversion::VoidCellManager::operator()(
        const McCAD::Conversion::VoidCellManager::membersMap& members,
        const Standard_Integer& depth, const Standard_Integer& width,
        const std::string& key, const aabbTuple& xAxisAABB,
        const aabbTuple& yAxisAABB, const aabbTuple& zAxisAABB){
    voidCell = std::make_shared<VoidCell>(depth, width, key);
    voidCell->xAxisUpdate = xAxisAABB;
    voidCell->yAxisUpdate = yAxisAABB;
    voidCell->zAxisUpdate = zAxisAABB;
    if (members.size() > 0) perform(members);
    return voidCell;
}

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

void
McCAD::Conversion::VoidCellManager::perform(
        const McCAD::Conversion::VoidCellManager::membersMap& members){
    populateLists(members);
    updateVoidCell(members);
    Standard_Boolean splitCondition = Standard_False;
    if (voidGeneration){
        // for depths > 1 you have to carry the dims of the half parent downstream to
        // update the final resultant void cells.
        if(!BVHVoid) voidCell->updateAABB();
        Standard_Boolean splitConditionOnNumSolids = members.size() > maxSolidsPerVoidCell
                ? Standard_True : Standard_False;
        Standard_Boolean splitConditionOnCellVol = voidCell->getAABBVolume() >= 2 * minVoidVolume
                ? Standard_True : Standard_False;
        splitCondition = splitConditionOnNumSolids && splitConditionOnCellVol;
    }
    //debug
    //voidCell->outputAABB();
    //debug
    if(splitCondition){
        auto surface = SplitSurfaceSelector{maxSolidsPerVoidCell, minVoidVolume
                                           }.process(xAxis, yAxis, zAxis, voidCell);
        if (!surface) return;
        auto splitMembers = splitVoidCell(*surface, members);
        voidCell->splitted = Standard_True;
        if(BVHVoid){
            // Create left void cell.
            auto voidCellLeft = VoidCellManager{BVHVoid, minVoidVolume,
                    maxSolidsPerVoidCell, voidGeneration}(
                        splitMembers.first, voidCell->depth + 1, 0, voidCell->key + "L");
            voidCell->daughterVoidCells.push_back(voidCellLeft);
            // Create right void cell.
            auto voidCellRight = VoidCellManager{BVHVoid, minVoidVolume,
                    maxSolidsPerVoidCell, voidGeneration}(
                        splitMembers.second, voidCell->depth + 1, 1, voidCell->key + "R");
            voidCell->daughterVoidCells.push_back(voidCellRight);
        } else{
            // Create left void cell.
            auto leftBoundaries = updateBoundaries(*surface, 0);
            auto voidCellLeft = VoidCellManager{BVHVoid, minVoidVolume,
                    maxSolidsPerVoidCell, voidGeneration}(
                        splitMembers.first, voidCell->depth + 1, 0, voidCell->key + "L",
                        leftBoundaries[0], leftBoundaries[1], leftBoundaries[2]);
            voidCell->daughterVoidCells.push_back(voidCellLeft);
            // Create right void cell.
            auto rightBoundaries = updateBoundaries(*surface, 1);
            auto voidCellRight = VoidCellManager{BVHVoid, minVoidVolume,
                    maxSolidsPerVoidCell, voidGeneration}(
                        splitMembers.second, voidCell->depth + 1, 1, voidCell->key + "R",
                        rightBoundaries[0], rightBoundaries[1], rightBoundaries[2]);
            voidCell->daughterVoidCells.push_back(voidCellRight);
        }
    }
}

void
McCAD::Conversion::VoidCellManager::populateLists(
        const McCAD::Conversion::VoidCellManager::membersMap& members){
    Standard_Real minX{0}, minY{0}, minZ{0}, maxX{0}, maxY{0}, maxZ{0};
    for(const auto& member : members){
        std::get<0>(member.second).Get(minX, minY, minZ, maxX, maxY, maxZ);
        xAxis[member.first] = std::make_tuple(minX, minX + std::abs(maxX-minX)/2.0, maxX);
        yAxis[member.first] = std::make_tuple(minY, minY + std::abs(maxY-minY)/2.0, maxY);
        zAxis[member.first] = std::make_tuple(minZ, minZ + std::abs(maxZ-minZ)/2.0, maxZ);
    }
}

void
McCAD::Conversion::VoidCellManager::updateVoidCell(
        const McCAD::Conversion::VoidCellManager::membersMap& members){
    voidCell->addSolidIDs(members);
    voidCell->addSolids(members);
}

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
        throw std::runtime_error("Chosen splitting surface is of an unknown type!.\n"
                                 "This is possibly caused by a code error, please report!.");
}

std::pair<McCAD::Conversion::VoidCellManager::membersMap,
          McCAD::Conversion::VoidCellManager::membersMap>
McCAD::Conversion::VoidCellManager::splitMembersList(
        const McCAD::Conversion::VoidCellManager::surfaceTuple& surface,
        const McCAD::Conversion::VoidCellManager::membersMap& members,
        const McCAD::Conversion::VoidCellManager::dimMap& axis){
    membersMap membersLeft, membersRight;
    std::string surfaceType = std::get<0>(surface);
    Standard_Real surfaceCoord = std::get<1>(surface);
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

McCAD::Conversion::VoidCellManager::aabbVec
McCAD::Conversion::VoidCellManager::updateBoundaries(
        McCAD::Conversion::VoidCellManager::surfaceTuple& surface,
        const Standard_Integer& sense){
    aabbTuple xAxisAABB, yAxisAABB, zAxisAABB;
    std::string surfaceType = std::get<0>(surface);
    Standard_Real surfaceCoord = std::get<1>(surface);
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
        throw std::runtime_error("Chosen splitting surface is of an unknown type!.\n"
                                 "This is possibly caused by a code error, please report!.");
     aabbVec xyzAABB{xAxisAABB, yAxisAABB, zAxisAABB};
     return xyzAABB;
}

Bnd_Box
McCAD::Conversion::VoidCellManager::updateOverlapAABB(
        const Bnd_Box& aAABB,
        const McCAD::Conversion::VoidCellManager::surfaceTuple& surface,
        const Standard_Integer& sense){
    Standard_Real surfaceCoord = std::get<1>(surface);
    Standard_Real minX{0}, minY{0}, minZ{0}, maxX{0}, maxY{0}, maxZ{0};
    aAABB.Get(minX, minY, minZ, maxX, maxY, maxZ);
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
