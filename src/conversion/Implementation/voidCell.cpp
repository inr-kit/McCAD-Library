// McCAD
#include "voidCell.hpp"
// OCC
#include <BRepBndLib.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <STEPControl_Writer.hxx>
#include <gp_Pnt.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>

McCAD::Conversion::VoidCell::VoidCell() : depth{0}, width{0}, key{"r"} {
}

McCAD::Conversion::VoidCell::VoidCell(const int& depth,
                                      const int& width,
                                      const std::string& key) :
    depth{depth}, width{width}, key{key}{
}

McCAD::Conversion::VoidCell::~VoidCell(){}

void
McCAD::Conversion::VoidCell::addSolidIDs(
        const McCAD::Conversion::VoidCell::membersMap& members){
    for(const auto& member : members){
        solidIDList.push_back(member.first);
    }
}

void
McCAD::Conversion::VoidCell::addSolids(
        const McCAD::Conversion::VoidCell::membersMap& members){
    for(const auto& member : members){
        aabb.Add(std::get<0>(member.second));
    }
    if(depth == 0) aabb.SetGap(100.0);
    else aabb.SetGap(0.0);
    aabb.Get(minX, minY, minZ, maxX, maxY, maxZ);
    // Create AABB solid
    gp_Pnt minPoint(minX, minY, minZ);
    gp_Pnt maxPoint(maxX, maxY, maxZ);
    aabbSolid = BRepPrimAPI_MakeBox(minPoint, maxPoint).Solid();
    // Create a lit of min, center, and max points of the AABB.
    xAxis = std::make_tuple(minX, minX + std::abs(maxX-minX)/2.0, maxX);
    yAxis = std::make_tuple(minY, minY + std::abs(maxY-minY)/2.0, maxY);
    zAxis = std::make_tuple(minZ, minZ + std::abs(maxZ-minZ)/2.0, maxZ);
}

void
McCAD::Conversion::VoidCell::updateAABB(){
    if(depth == 0) return;
    Bnd_Box newAABB;
    minX = std::get<0>(xAxisUpdate);
    maxX = std::get<1>(xAxisUpdate);
    minY = std::get<0>(yAxisUpdate);
    maxY = std::get<1>(yAxisUpdate);
    minZ = std::get<0>(zAxisUpdate);
    maxZ = std::get<1>(zAxisUpdate);
    gp_Pnt minPoint(minX, minY, minZ);
    gp_Pnt maxPoint(maxX, maxY, maxZ);
    newAABB.Add(minPoint);
    newAABB.Add(maxPoint);
    aabb = newAABB;
    aabbSolid = BRepPrimAPI_MakeBox(minPoint, maxPoint).Solid();
    // Update lists of min, center, and max points of the AABB.
    xAxis = std::make_tuple(minX, minX + std::abs(maxX-minX)/2.0, maxX);
    yAxis = std::make_tuple(minY, minY + std::abs(maxY-minY)/2.0, maxY);
    zAxis = std::make_tuple(minZ, minZ + std::abs(maxZ-minZ)/2.0, maxZ);
}

double
McCAD::Conversion::VoidCell::getAABBVolume(){
    GProp_GProps geometryProperties;
    BRepGProp::VolumeProperties(aabbSolid, geometryProperties);
    return geometryProperties.Mass();
    // double volume = std::abs(maxX - minX) * std::abs(maxY - minY) *
    //        std::abs(maxZ - minZ);
    //return volume;
}

void
McCAD::Conversion::VoidCell::outputAABB(){
    STEPControl_Writer writer0;
    writer0.Transfer(aabbSolid, STEPControl_StepModelType::STEPControl_AsIs);
    std::string filename = "./aabb";
    std::string suffix = ".stp";
    filename += "_" + key + std::to_string(depth) + std::to_string(width);
    filename += suffix;
    writer0.Write(filename.c_str());
}
