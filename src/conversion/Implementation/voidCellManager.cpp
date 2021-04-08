// McCAD
#include "voidCellManager.hpp"
#include "TaskQueue.hpp"
#include "splitSurfaceSelector.hpp"
// OCC
#include "STEPControl_Writer.hxx"
#include "BRepPrimAPI_MakeBox.hxx"

McCAD::Conversion::VoidCellManager::VoidCellManager(){}

McCAD::Conversion::VoidCellManager::~VoidCellManager(){}

void
McCAD::Conversion::VoidCellManager::operator()(
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList,
        const Standard_Integer& maxSolidsPerVoidCell){
    voidCell = std::make_shared<VoidCell>();
    perform(solidObjList, maxSolidsPerVoidCell);
}

void
McCAD::Conversion::VoidCellManager::operator()(
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList,
        const Standard_Integer& maxSolidsPerVoidCell,
        const Standard_Integer& depth, const Standard_Integer& width){
    voidCell = std::make_shared<VoidCell>(depth, width);
    perform(solidObjList, maxSolidsPerVoidCell);
}

void
McCAD::Conversion::VoidCellManager::perform(
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList,
        const Standard_Integer& maxSolidsPerVoidCell){
    populateLists(solidObjList);
    Standard_Boolean splitCondition = solidObjList.size() > maxSolidsPerVoidCell
            ? Standard_True : Standard_False;
    updateVoidCell(solidObjList);
    if(splitCondition){
        auto surface = SplitSurfaceSelector{maxSolidsPerVoidCell}.process(xAxis, yAxis, zAxis, voidCell);
        voidCell->split = Standard_True;
        splitVoidCell(surface);
        }
}

void
McCAD::Conversion::VoidCellManager::populateLists(
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList){
    Standard_Real minX{0}, minY{0}, minZ{0}, maxX{0}, maxY{0}, maxZ{0};
    for(const auto& solidObj : solidObjList){
        auto solidID = solidObj->accessSImpl()->solidID;
        solidObj->accessSImpl()->aabb.Get(minX, minY, minZ, maxX, maxY, maxZ);
        xAxis.push_back(std::make_tuple(solidID, minX, minX + std::abs(maxX-minX)/2.0, maxX));
        yAxis.push_back(std::make_tuple(solidID, minY, minY + std::abs(maxY-minY)/2.0, maxY));
        zAxis.push_back(std::make_tuple(solidID, minZ, minZ + std::abs(maxZ-minZ)/2.0, maxZ));
    }
}

void
McCAD::Conversion::VoidCellManager::updateVoidCell(
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList){
    voidCell->addSolidIDs(solidObjList);
    voidCell->addSolids(solidObjList);
}

void
McCAD::Conversion::VoidCellManager::splitVoidCell(
        const McCAD::Conversion::VoidCellManager::surfaceTuple& surface){
    if (std::get<0>(surface) == "X"){
        std::cout << "split by x" << std::endl;
    } else if((std::get<0>(surface) == "Y")){
        std::cout << "split by y" << std::endl;
    } else{
        std::cout << "split by z" << std::endl;
    }
    // call cell manager on split lists
}
