// McCAD
#include "voidCell.hpp"
// OCC
#include <BRepBndLib.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <STEPControl_Writer.hxx>
#include <gp_Pnt.hxx>

McCAD::Conversion::VoidCell::VoidCell()
    : depth{0}, width{0} {
}

McCAD::Conversion::VoidCell::VoidCell(const Standard_Integer& depth,
                                      const Standard_Integer& width)
    : depth{depth}, width{width}{
}

McCAD::Conversion::VoidCell::~VoidCell(){}

void
McCAD::Conversion::VoidCell::addSolidIDs(const std::vector<std::shared_ptr<
                                         Geometry::Solid>>& solidObjList){
    for(auto& solid : solidObjList){
        solidIDList.push_back(solid->accessSImpl()->solidID);
    }
}

void
McCAD::Conversion::VoidCell::addSolids(
        const std::vector<std::shared_ptr<Geometry::Solid>>& solidObjList){
    for(auto& solid : solidObjList){
        aabb.Add(solid->accessSImpl()->aabb);
        //BRepBndLib::AddOptimal(solid->accessSImpl()->aabb, aabb);
    }
    if(depth == 0.0) aabb.SetGap(1.0);
    else aabb.SetGap(0.0);
    aabb.Get(minX, minY, minZ, maxX, maxY, maxZ);
    gp_Pnt minPoint(minX, minY, minZ);
    gp_Pnt maxPoint(maxX, maxY, maxZ);
    aabbSolid = BRepPrimAPI_MakeBox(minPoint, maxPoint).Solid();
    ///*//debug
    std::cout << "VoidCell contains: " << solidIDList.size() << std::endl;
    STEPControl_Writer writer0;
    writer0.Transfer(aabbSolid, STEPControl_StepModelType::STEPControl_AsIs);
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
    // Create a lit of min, center, and max points of the AABB.
    xAxis = std::make_tuple(minX, minX + std::abs(maxX-minX)/2.0, maxX);
    yAxis = std::make_tuple(minY, minY + std::abs(maxY-minY)/2.0, maxY);
    zAxis = std::make_tuple(minZ, minZ + std::abs(maxZ-minZ)/2.0, maxZ);
}
