// McCAD
#include "voidCellManager.hpp"
#include "TaskQueue.hpp"
#include "splitSurfaceSelector.hpp"

McCAD::Conversion::VoidCellManager::VoidCellManager(
        const Standard_Boolean& BVH, const Standard_Integer& maxSolidsPerVoidCell) :
    BVH{BVH}, maxSolidsPerVoidCell{maxSolidsPerVoidCell}{
}

McCAD::Conversion::VoidCellManager::~VoidCellManager(){}

std::shared_ptr<McCAD::Conversion::VoidCell>
McCAD::Conversion::VoidCellManager::operator()(
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList){
    voidCell = std::make_shared<VoidCell>();
    auto members = createLists(solidObjList);
    perform(members);
    return voidCell;
}

std::shared_ptr<McCAD::Conversion::VoidCell>
McCAD::Conversion::VoidCellManager::operator()(
        const McCAD::Conversion::VoidCellManager::membersMap& members,
        const Standard_Integer& depth, const Standard_Integer& width){
    voidCell = std::make_shared<VoidCell>(depth, width);
    perform(members);
    return voidCell;
}

std::shared_ptr<McCAD::Conversion::VoidCell>
McCAD::Conversion::VoidCellManager::operator()(
        const McCAD::Conversion::VoidCellManager::membersMap& members,
        const Standard_Integer& depth, const Standard_Integer& width,
        const aabbTuple& xAxis, const aabbTuple& yAxis, const aabbTuple& zAxis){
    voidCell = std::make_shared<VoidCell>(depth, width);
    voidCell->xAxisUpdate = xAxis;
    voidCell->yAxisUpdate = yAxis;
    voidCell->zAxisUpdate = zAxis;
    perform(members);
    return voidCell;
}

McCAD::Conversion::VoidCellManager::membersMap
McCAD::Conversion::VoidCellManager::createLists(
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList){
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
    // for depths > 1 you have to carry the dims of the half parent downstream to
    // update the final resultant void cells.
    updateVoidCell(members);
    if(!BVH) voidCell->updateAABB();
    Standard_Boolean splitCondition = members.size() > maxSolidsPerVoidCell
                                      ? Standard_True : Standard_False;
    if(splitCondition){
        voidCell->splitted = Standard_True;
        auto surface = SplitSurfaceSelector{maxSolidsPerVoidCell}.process(
                    xAxis, yAxis, zAxis, voidCell);
        std::cout << "Chosen splitting surface: " << std::get<0>(surface) <<
                     ", " << std::get<1>(surface) << ", " << std::get<2>(surface)
                  << ", " << std::get<3>(surface) << std::endl;
        auto splitMembers = splitVoidCell(surface, members);
        if(BVH){
            auto voidCellLeft = VoidCellManager{BVH, maxSolidsPerVoidCell}(
                        splitMembers.first, voidCell->depth + 1, 0);
            voidCell->daughterVoidCells.push_back(voidCellLeft);
            auto voidCellRight = VoidCellManager{BVH, maxSolidsPerVoidCell}(
                        splitMembers.second, voidCell->depth + 1, 1);
            voidCell->daughterVoidCells.push_back(voidCellRight);
        } else{
            auto voidCellLeft = VoidCellManager{BVH, maxSolidsPerVoidCell}(
                        splitMembers.first, voidCell->depth + 1, 0,
                        voidCell->xAxis, voidCell->yAxis, voidCell->ZAxis);
            //else updateBoundaries(surface, voidCellLeft, 0);
            voidCell->daughterVoidCells.push_back(voidCellLeft);
            auto voidCellRight = VoidCellManager{BVH, maxSolidsPerVoidCell}(
                        splitMembers.second, voidCell->depth + 1, 1,
                        voidCell->xAxis, voidCell->yAxis, voidCell->ZAxis);
            //if(!BVH) updateBoundaries(surface, voidCellRight, 1);
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

void
McCAD::Conversion::VoidCellManager::updateBoundaries(
        const McCAD::Conversion::VoidCellManager::surfaceTuple& surface,
        const std::shared_ptr<McCAD::Conversion::VoidCell>& voidCellDaughter,
        const Standard_Integer& sense){
    std::string surfaceType = std::get<0>(surface);
    Standard_Real surfaceCoord = std::get<1>(surface);
    if (surfaceType == "X"){
        voidCellDaughter->minY = voidCell->minY;
        voidCellDaughter->maxY = voidCell->maxY;
        voidCellDaughter->minZ = voidCell->minZ;
        voidCellDaughter->maxZ = voidCell->maxZ;
        if(sense == 0) {
            voidCellDaughter->minX = voidCell->minX;
            voidCellDaughter->maxX = surfaceCoord;
        } else{
            voidCellDaughter->minX = surfaceCoord;
            voidCellDaughter->maxX = voidCell->maxX;
        }
    } else if(surfaceType == "Y"){
        voidCellDaughter->minX = voidCell->minX;
        voidCellDaughter->maxX = voidCell->maxX;
        voidCellDaughter->minZ = voidCell->minZ;
        voidCellDaughter->maxZ = voidCell->maxZ;
        if(sense == 0) {
            voidCellDaughter->minY = voidCell->minY;
            voidCellDaughter->maxY = surfaceCoord;
        } else{
            voidCellDaughter->minY = surfaceCoord;
            voidCellDaughter->maxY = voidCell->maxY;
        }
    } else if(surfaceType == "Z"){
        voidCellDaughter->minX = voidCell->minX;
        voidCellDaughter->maxX = voidCell->maxX;
        voidCellDaughter->minY = voidCell->minY;
        voidCellDaughter->maxY = voidCell->maxY;
        if(sense == 0) {
            voidCellDaughter->minZ = voidCell->minZ;
            voidCellDaughter->maxZ = surfaceCoord;
        } else{
            voidCellDaughter->minZ = surfaceCoord;
            voidCellDaughter->maxZ = voidCell->maxZ;
        }
    } else
        throw std::runtime_error("Chosen splitting surface is of an unknown type!.\n"
                                 "This is possibly caused by a code error, please report!.");
     voidCellDaughter->updateAABB();
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
