// McCAD
#include "voidCellManager.hpp"
#include "TaskQueue.hpp"
#include "splitSurfaceSelector.hpp"

McCAD::Conversion::VoidCellManager::VoidCellManager(){}

McCAD::Conversion::VoidCellManager::~VoidCellManager(){}

void
McCAD::Conversion::VoidCellManager::operator()(
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList,
        const Standard_Integer& maxSolidsPerVoidCell){
    voidCell = std::make_shared<VoidCell>();
    auto members = createLists(solidObjList);
    perform(members, maxSolidsPerVoidCell);
}

void
McCAD::Conversion::VoidCellManager::operator()(
        const McCAD::Conversion::VoidCellManager::membersMap& members,
        const Standard_Integer& maxSolidsPerVoidCell, const Standard_Integer& depth,
        const Standard_Integer& width){
    voidCell = std::make_shared<VoidCell>(depth, width);
    perform(members, maxSolidsPerVoidCell);
}

McCAD::Conversion::VoidCellManager::membersMap
McCAD::Conversion::VoidCellManager::createLists(
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList){
    McCAD::Conversion::VoidCellManager::membersMap members;
    for(const auto& solidObj : solidObjList){
        members[solidObj->accessSImpl()->solidID] =
                std::make_tuple(solidObj->accessSImpl()->aabb, "NA", 0.0);
    }
    return members;
}

void
McCAD::Conversion::VoidCellManager::perform(
        const McCAD::Conversion::VoidCellManager::membersMap& members,
        const Standard_Integer& maxSolidsPerVoidCell){
    populateLists(members);
    Standard_Boolean splitCondition = members.size() > maxSolidsPerVoidCell
            ? Standard_True : Standard_False;
    updateVoidCell(members);
    if(splitCondition){
        voidCell->split = Standard_True;
        auto surface = SplitSurfaceSelector{maxSolidsPerVoidCell}.process(xAxis, yAxis, zAxis, voidCell);
        auto splitMembers = splitVoidCell(surface, members);
        std::cout << "Left: " << (splitMembers.first).size() <<
                     "\nRight:" << (splitMembers.second).size() << std::endl;
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
    } else{
        return splitMembersList(surface, members, zAxis);
    }
}

std::pair<McCAD::Conversion::VoidCellManager::membersMap,
          McCAD::Conversion::VoidCellManager::membersMap>
McCAD::Conversion::VoidCellManager::splitMembersList(
        const McCAD::Conversion::VoidCellManager::surfaceTuple& surface,
        const McCAD::Conversion::VoidCellManager::membersMap& members,
        const McCAD::Conversion::VoidCellManager::dimMap& axis){
    McCAD::Conversion::VoidCellManager::membersMap membersLeft, membersRight;
    std::string surfaceType = std::get<0>(surface);
    Standard_Real surfaceCoord = std::get<1>(surface);
    for(const auto& member : members){
        auto ID = member.first;
        auto dimensions = axis.at(ID);
        if(std::get<0>(dimensions) >= surfaceCoord){
            // Add to solid on the right.
            membersRight[ID] = std::make_tuple(std::get<0>(member.second), "NA", 0.0);
        } else if(std::get<2>(dimensions) <= surfaceCoord){
            // Add to solid on the left.
            membersLeft[member.first] = std::make_tuple(std::get<0>(member.second), "NA", 0.0);
        } else {
            // Solid overlaps, add to both lists.
            membersRight[ID] = std::make_tuple(std::get<0>(member.second),
                                               surfaceType, surfaceCoord);
            membersLeft[ID] = std::make_tuple(std::get<0>(member.second),
                                              surfaceType, surfaceCoord);
        }
    }
    return std::make_pair(membersLeft, membersRight);
}
