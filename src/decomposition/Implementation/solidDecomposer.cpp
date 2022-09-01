// C++
#include <array>
#include <filesystem>
// McCAD
#include "solidDecomposer.hpp"
#include "SolidSplitter.hpp"
#include "ShapeView.hpp"
#include "SurfaceUtilities.hpp"
#include "SolidRepairer.hpp"
//OCCT
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <STEPControl_Writer.hxx>

McCAD::Decomposition::SolidDecomposer::SolidDecomposer(const int & debugLevel) 
    : debugLevel{debugLevel} {
}

McCAD::Decomposition::SolidDecomposer::~SolidDecomposer(){
}

/** ********************************************************************
* @brief    A function that decomposes a given solid.
* @param    solid is a OCCT solid.
* @param    obb is a OCCT oriented bounding box.
* @param    surface is a McCAD surface object.
* @param    subSolidsList is a list of OCCT solids to be filled after successful splitting.
* @return   true if decomposition succeded.
* @date     31/12/2020
* @modified 31/08/2022
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::SolidDecomposer::operator()(
        const TopoDS_Solid& solid,
        const Bnd_OBB& obb,
        const Geometry::BoundSurface& surface,
        TopTools_HSequenceOfShape& subSolidsList) const{
    if(surface.getSurfaceType() != Tools::toTypeName(GeomAbs_Plane))
        return false;
    auto halfSolids = SolidSplitter{ debugLevel }(solid, obb, surface.accessSImpl()->extendedFace);
    if(!halfSolids) return false;
    subSolidsList.Append(halfSolids->first);
    subSolidsList.Append(halfSolids->second);
    //debug
    if (debugLevel >= 2) {
        STEPControl_Writer writer;
        writer.Transfer(halfSolids->first, STEPControl_StepModelType::STEPControl_AsIs);
        writer.Transfer(halfSolids->second, STEPControl_StepModelType::STEPControl_AsIs);
        int kk = 0;
        std::string filename = "SolidDecomposer_Result";
        std::string suffix = ".stp";
        while (std::filesystem::exists(filename + std::to_string(kk) + suffix)) {
            ++kk;
        }
        filename += std::to_string(kk);
        filename += suffix;
        writer.Write(filename.c_str());
    }
    //debug
    return filterAndRepair(subSolidsList);
}

bool
McCAD::Decomposition::SolidDecomposer::filterAndRepair(
        TopTools_HSequenceOfShape& subSolidsList,
        double tolerance) const{
    auto filteredSubSolids = gatherSubSolids(subSolidsList, tolerance);
    auto success = SolidRepairer{}(filteredSubSolids);
    if(success){
        if(filteredSubSolids.Length() > 1) subSolidsList = filteredSubSolids;
        else success = false;
    }
    return success;
}

TopTools_HSequenceOfShape
McCAD::Decomposition::SolidDecomposer::gatherSubSolids(
        TopTools_HSequenceOfShape& solids,
        double tolerance) const{
    TopTools_HSequenceOfShape subSolids;
    for(const auto& solid : solids){
        for(const auto& subSolid : detail::ShapeView<TopAbs_SOLID>{solid}){
            if(subSolid.IsNull()) continue;
            GProp_GProps geoProps;
            BRepGProp::VolumeProperties(subSolid, geoProps);
            if(geoProps.Mass() <= tolerance) continue;
            subSolids.Append(subSolid);
        }
    }
    return subSolids;
}

