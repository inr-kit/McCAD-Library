// McCAD
#include "voidCellManager.hpp"
#include "TaskQueue.hpp"
#include "splitSurfaceSelector.hpp"

McCAD::Conversion::VoidCellManager::VoidCellManager(
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList,
        const Standard_Integer& maxSolidsPerVoidCell){
    voidCell = std::make_shared<VoidCell>();
    populateLists(solidObjList);
    Standard_Boolean splitCondition = solidObjList.size() > maxSolidsPerVoidCell
            ? Standard_True : Standard_False;
    if(splitCondition){
        voidCell->split = splitVoidCell(maxSolidsPerVoidCell);
        //addDaughters();
    }
    else updateVoidCell(voidCell, solidObjList);
}

McCAD::Conversion::VoidCellManager::~VoidCellManager(){}

void
McCAD::Conversion::VoidCellManager::populateLists(
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList){
    Standard_Real minX{0}, minY{0}, minZ{0}, maxX{0}, maxY{0}, maxZ{0}, center{0};
    for(const auto& solidObj : solidObjList){
        auto solidID = solidObj->accessSImpl()->solidID;
        solidObj->accessSImpl()->aabb.Get(minX, minY, minZ, maxX, maxY, maxZ);
        xAxis.push_back(std::make_tuple(solidID, minX, minX + std::abs(maxX-minX)/2.0, maxX));
        yAxis.push_back(std::make_tuple(solidID, minY, minY + std::abs(maxY-minY)/2.0, maxY));
        zAxis.push_back(std::make_tuple(solidID, minZ, minZ + std::abs(maxZ-minZ)/2.0, maxZ));
    }
}

Standard_Boolean
McCAD::Conversion::VoidCellManager::splitVoidCell(const Standard_Integer& maxSolidsPerVoidCell){
    SplitSurfaceSelector{}.process(xAxis, yAxis, zAxis);
    return Standard_False;
}

void
McCAD::Conversion::VoidCellManager::updateVoidCell(
        const std::shared_ptr<VoidCell>& voidCell,
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList){
    voidCell->addSolidIDs(solidObjList);
}
