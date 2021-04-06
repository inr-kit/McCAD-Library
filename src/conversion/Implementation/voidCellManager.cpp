// McCAD
#include "voidCellManager.hpp"
#include "TaskQueue.hpp"
#include "splitSurfaceSelector.hpp"
// OCC
#include "STEPControl_Writer.hxx"
#include "BRepPrimAPI_MakeBox.hxx"

McCAD::Conversion::VoidCellManager::VoidCellManager(
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList,
        const Standard_Integer& maxSolidsPerVoidCell) :
    voidCell{std::make_shared<VoidCell>()}{
    populateLists(solidObjList);
    Standard_Boolean splitCondition = solidObjList.size() > maxSolidsPerVoidCell
            ? Standard_True : Standard_False;
    updateVoidCell(solidObjList);
    if(splitCondition){
        voidCell->split = splitVoidCell(maxSolidsPerVoidCell);
        //addDaughters();
    }
}

McCAD::Conversion::VoidCellManager::~VoidCellManager(){}

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
        ///*//debug
        gp_Pnt minPoint(minX, minY, minZ);
        gp_Pnt maxPoint(maxX, maxY, maxZ);
        //auto aabbSolid = BRepPrimAPI_MakeBox(minPoint, maxPoint).Solid();
        STEPControl_Writer writer0;
        writer0.Transfer(BRepPrimAPI_MakeBox(minPoint, maxPoint).Solid(),
                         STEPControl_StepModelType::STEPControl_AsIs);
        Standard_Integer kk = 0;
        std::string filename = "./aabb";
        std::string suffix = ".stp";
        while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
            ++kk;
        }
        filename += std::to_string(kk);
        filename += suffix;
        writer0.Write(filename.c_str());
        //*///debug
    }
}

Standard_Boolean
McCAD::Conversion::VoidCellManager::splitVoidCell(const Standard_Integer& maxSolidsPerVoidCell){
    SplitSurfaceSelector{}.process(xAxis, yAxis, zAxis, voidCell);
    return Standard_False;
}

void
McCAD::Conversion::VoidCellManager::updateVoidCell(
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList){
    voidCell->addSolidIDs(solidObjList);
    voidCell->addSolids(solidObjList);
}
