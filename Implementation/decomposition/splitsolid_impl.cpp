// C++
#include <iterator>
// McCAD
#include "splitsolid_impl.hpp"
#include "SolidSplitter.hpp"
#include "SolidRepairer.hpp"
#include "SurfaceUtilities.hpp"
//OCC
#include "GeomAbs_SurfaceType.hxx"

Standard_Boolean
McCAD::Decomposition::SplitSolid::Impl::operator()(
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
    return filterAndRepair(subSolidsList);
}

Standard_Boolean
McCAD::Decomposition::SplitSolid::Impl::filterAndRepair(
        TopTools_HSequenceOfShape& subSolidsList,
        Standard_Real tolerance) const{
    auto filteredSubSolids = gatherSubSolids(subSolidsList, tolerance);
    auto success = SolidRepairer{}(filteredSubSolids);
    if(success) subSolidsList = filteredSubSolids;
    return success;
}

TopTools_HSequenceOfShape
McCAD::Decomposition::SplitSolid::Impl::gatherSubSolids(
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

