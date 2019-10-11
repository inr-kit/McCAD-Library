#include "SolidRebuilder.hpp"

// OCC
#include <ShapeFix_Solid.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>

// McCAD
#include "ShapeView.hpp"

McCAD::Decomposition::SolidRebuilder::SolidRebuilder(
        Standard_Real faceMassTolerance,
        Standard_Real sewingTolerance,
        Standard_Real fixingTolerance)
    : faceMassTolerance{faceMassTolerance}
    , sewingTolerance{sewingTolerance}
    , fixingTolerance{fixingTolerance}{
}

std::optional<TopoDS_Solid>
McCAD::Decomposition::SolidRebuilder::operator()(
        const TopoDS_Solid& originalSolid) const{

    auto faces = solidToFaces(originalSolid);
    if(faces.empty()) return std::nullopt;

    auto shell = shapeToSingleShell(facesToShape(faces));
    if(!shell.has_value()) return std::nullopt;

    return shellToSolid(*shell);
}

std::vector<const TopoDS_Face*>
McCAD::Decomposition::SolidRebuilder::solidToFaces(
        const TopoDS_Solid& solid) const{
    std::vector<const TopoDS_Face*> faces;

    for(const auto& face : ShapeView<TopAbs_FACE>{solid}){
        if(face.IsNull()) continue;
        GProp_GProps geometryProperties;
        BRepGProp::SurfaceProperties(face, geometryProperties);
        if(geometryProperties.Mass() <= faceMassTolerance) continue;
        faces.push_back(&face);
    }

    return faces;
}

TopoDS_Shape
McCAD::Decomposition::SolidRebuilder::facesToShape(
        const std::vector<const TopoDS_Face*>& faces)const {
    BRepBuilderAPI_Sewing builder{
        sewingTolerance,
        Standard_True, Standard_True, Standard_True, Standard_True
    };

    for(const auto& face : faces)
        builder.Add(*face);

    builder.Perform();
    return builder.SewedShape();
}

std::optional<TopoDS_Shell>
McCAD::Decomposition::SolidRebuilder::shapeToSingleShell(
        const TopoDS_Shape& shape) const{

    ShapeView<TopAbs_SHELL> shapeView{shape};
    if(shapeView.size() == 1)
        return *shapeView.begin();

    return std::nullopt;
}

std::optional<TopoDS_Solid>
McCAD::Decomposition::SolidRebuilder::shellToSolid(
        const TopoDS_Shell& shell) const{
    ShapeFix_Solid shapeFix;
    shapeFix.LimitTolerance(fixingTolerance);
    auto solid = shapeFix.SolidFromShell(shell);

    if(BRepCheck_Analyzer{solid, Standard_True}.IsValid())
        return solid;
    else
        return std::nullopt;
}
