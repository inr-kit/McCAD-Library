// McCAD
#include "voidCellManager.hpp"
#include "TaskQueue.hpp"
#include "solidsSorter.hpp"

McCAD::Conversion::VoidCellManager::VoidCellManager(
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList){
    populateLists(solidObjList);
    McCAD::Conversion::SolidsSorter{}.sortByElement(xAxis, 1);
    std::cout << "x: " << xAxis.size() << std::endl;
    for(const auto& i : xAxis){
        std::cout << std::get<0>(i) << ", " << std::get<1>(i) << ", " <<
                     std::get<2>(i) << ", " << std::get<3>(i) << std::endl;
    }
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
