// C++
#include <array>
#include <filesystem>
// McCAD
#include "solidDecomposer.hpp"
#include "SolidSplitter.hpp"
#include "ShapeView.hpp"
#include "SurfaceUtilities.hpp"
#include "SolidRepairer.hpp"
//OCC
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <STEPControl_Writer.hxx>

McCAD::Decomposition::SolidDecomposer::SolidDecomposer(){
}

McCAD::Decomposition::SolidDecomposer::~SolidDecomposer(){
}

Standard_Boolean
McCAD::Decomposition::SolidDecomposer::operator()(
        const TopoDS_Solid& solid,
        const Bnd_OBB& obb,
        const Geometry::BoundSurface& surface,
        TopTools_HSequenceOfShape& subSolidsList) const{
    if(surface.getSurfaceType() != Tools::toTypeName(GeomAbs_Plane))
        return Standard_False;
    auto halfSolids = SolidSplitter{}(solid, obb, surface.accessSImpl()->extendedFace);
    if(!halfSolids) return Standard_False;
    subSolidsList.Append(halfSolids->first);
    subSolidsList.Append(halfSolids->second);
    /*//debug
    STEPControl_Writer writer0;
    writer0.Transfer(halfSolids->first, STEPControl_StepModelType::STEPControl_AsIs);
    writer0.Transfer(halfSolids->second, STEPControl_StepModelType::STEPControl_AsIs);
    Standard_Integer kk = 0;
    std::string filename = "solidSplitting";
    std::string suffix = ".stp";
    while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
        ++kk;
    }
    filename += std::to_string(kk);
    filename += suffix;
    writer0.Write(filename.c_str());
    *///debug
    return filterAndRepair(subSolidsList);
}

Standard_Boolean
McCAD::Decomposition::SolidDecomposer::filterAndRepair(
        TopTools_HSequenceOfShape& subSolidsList,
        Standard_Real tolerance) const{
    auto filteredSubSolids = gatherSubSolids(subSolidsList, tolerance);
    auto success = SolidRepairer{}(filteredSubSolids);
    if(success) subSolidsList = filteredSubSolids;
    return success;
}

TopTools_HSequenceOfShape
McCAD::Decomposition::SolidDecomposer::gatherSubSolids(
        TopTools_HSequenceOfShape& solids,
        Standard_Real tolerance) const{
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

