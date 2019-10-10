// McCAD
#include "splitsolid_impl.hpp"
#include "SolidSplitter.hpp"
#include "SolidRepairer.hpp"

// C++
#include <iterator>

bool
McCAD::Decomposition::SplitSolid::Impl::operator()(
        const TopoDS_Solid& solid,
        const Geometry::BoundSurface& surface,
        TopTools_HSequenceOfShape& subSolidsList) const{
    if(surface.getSurfaceType() != "Plane") return false;

    auto halfSolids = SolidSplitter{}(solid, surface.accessSImpl()->extendedFace);
    if(!halfSolids) return false;

    subSolidsList.Append(halfSolids->first);
    subSolidsList.Append(halfSolids->second);

    return filterAndRepair(subSolidsList);
}

bool
McCAD::Decomposition::SplitSolid::Impl::filterAndRepair(
        TopTools_HSequenceOfShape& subSolidsList,
        Standard_Real tolerance) const{
    auto filteredSubSolids = gatherSubSolids(subSolidsList, tolerance);

    auto success = SolidRepairer{}(filteredSubSolids);
    if(success)
        subSolidsList = filteredSubSolids;

    return success;
}

TopTools_HSequenceOfShape
McCAD::Decomposition::SplitSolid::Impl::gatherSubSolids(
        TopTools_HSequenceOfShape& solids,
        Standard_Real tolerance) const{
    TopTools_HSequenceOfShape subSolids;

    for(const auto& solid : solids){
        for(const auto& subSolid : ShapeView<TopAbs_SOLID>{solid}){
            if(subSolid.IsNull()) continue;

            GProp_GProps geoProps;
            BRepGProp::VolumeProperties(subSolid, geoProps);
            // Is this how it is meant?
            // In the pre-refactoring code I found that the VolumeProperties of "solid"
            // instead of "subSolid" are taken. I guess, "subSolid" makes more sense here.

            if(geoProps.Mass() <= tolerance) continue;
            subSolids.Append(subSolid);
        }
    }
    return subSolids;
}

#include <BRepBuilderAPI_Transform.hxx>

void
McCAD::Decomposition::SplitSolid::Impl::createOBBSolid(const Bnd_OBB& OBB){
    bndBox = OBB;
    bndBox.Enlarge(0.4);
    boxSquareLength = sqrt(bndBox.SquareExtent());
    std::array<gp_Pnt, 8> corners;
    bndBox.GetVertex(&corners[0]);

    gp_Trsf transformation;
    transformation.SetTransformation(
                gp_Ax3{gp_Pnt(0, 0, 0),
                bndBox.ZDirection(),
                bndBox.XDirection()});

    boundingBox = BRepPrimAPI_MakeBox{
            corners[0].Transformed(transformation.Inverted()),
            corners[7].Transformed(transformation.Inverted())
        }.Solid();

    BRepBuilderAPI_Transform boxTransform{transformation};
    boxTransform.Perform(boundingBox);
    boundingBox = boxTransform.ModifiedShape(boundingBox);
}


