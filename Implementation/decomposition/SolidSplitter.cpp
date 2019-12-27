//McCAD
#include "SolidSplitter.hpp"
// C++
#include <array>
#include <filesystem>
// OCC
#include <BRepAdaptor_Surface.hxx>
#include <BRepBndLib.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeHalfSpace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <gp_Pln.hxx>
#include <STEPControl_Writer.hxx>

std::optional<std::pair<TopoDS_Shape, TopoDS_Shape>>
McCAD::Decomposition::SolidSplitter::operator()(
        const TopoDS_Solid& solidToSplit, const Bnd_OBB& obb,
        const TopoDS_Face& splittingFace) const{
    auto boundingBox = calculateOBB(obb);
    /* //debug
    STEPControl_Writer writer0;
    writer0.Transfer(boundingBox, STEPControl_StepModelType::STEPControl_AsIs);
    writer0.Transfer(solidToSplit, STEPControl_StepModelType::STEPControl_AsIs);
    writer0.Transfer(splittingFace, STEPControl_StepModelType::STEPControl_AsIs);
    std::cout << "OBB sqrt square extent: " << sqrt(obb.SquareExtent()) << std::endl;
    Standard_Integer kk = 0;
    std::string filename = "/home/mharb/Documents/McCAD_refactor/examples/bbox/solid";
    std::string suffix = ".stp";
    while (std::filesystem::exists(filename + std::to_string(kk) + suffix)){
        ++kk;
    }
    filename += std::to_string(kk);
    filename += suffix;
    writer0.Write(filename.c_str());
    */ //debug
    auto halfBoundingBoxes = calculateHalfBB(splittingFace, boundingBox);
    if(!halfBoundingBoxes) return std::nullopt;
    return calculateHalfSolids(solidToSplit, *halfBoundingBoxes);
}

TopoDS_Shape
McCAD::Decomposition::SolidSplitter::calculateOBB(
        Bnd_OBB obb, Standard_Real clearanceTolerance) const{
    obb.Enlarge(clearanceTolerance);
    std::array<gp_Pnt, 8> corners;
    obb.GetVertex(&corners[0]);
    gp_Trsf transformation;
    transformation.SetTransformation(gp_Ax3{gp_Pnt(0, 0, 0), obb.ZDirection(),
                                            obb.XDirection()});
    auto boundingBox = BRepPrimAPI_MakeBox{
            corners[0].Transformed(transformation),
            corners[7].Transformed(transformation)}.Shape();
    BRepBuilderAPI_Transform boxTransform{transformation.Inverted()};
    boxTransform.Perform(boundingBox);
    return boxTransform.ModifiedShape(boundingBox);
}

std::pair<gp_Pnt, gp_Pnt>
McCAD::Decomposition::SolidSplitter::calculatePoints(
        const TopoDS_Face& splittingFace) const{

    auto surfaceAdaptor = BRepAdaptor_Surface{splittingFace, Standard_True}.Surface();
    if (surfaceAdaptor.GetType() == GeomAbs_Plane){
        const auto& position = surfaceAdaptor.Plane().Position();
        const auto& direction = position.Direction();
        const auto& location = position.Location();

        return {
            location.Translated(+100 * direction),
            location.Translated(-100 * direction)
        };
    }
    return {};
}

std::pair<TopoDS_Solid, TopoDS_Solid>
McCAD::Decomposition::SolidSplitter::calculateHalfSpaces(
        const TopoDS_Face& splittingFace) const{
    auto points = calculatePoints(splittingFace);

    return {
        BRepPrimAPI_MakeHalfSpace{splittingFace, points.first}.Solid(),
        BRepPrimAPI_MakeHalfSpace{splittingFace, points.second}.Solid()
    };
}

std::optional<std::pair<TopoDS_Shape, TopoDS_Shape>>
McCAD::Decomposition::SolidSplitter::calculateHalfBB(
        const TopoDS_Face& splittingFace,
        const TopoDS_Shape& boundingBox) const{
    auto halfSpaces = calculateHalfSpaces(splittingFace);

    auto firstHalfBB = calculateHalfOBB(halfSpaces.first, boundingBox);
    if(!firstHalfBB) return std::nullopt;

    auto secondHalfBB = calculateHalfOBB(halfSpaces.second, boundingBox);
    if(!secondHalfBB) return std::nullopt;

    return std::make_pair(*firstHalfBB, *secondHalfBB);
}

std::optional<TopoDS_Shape>
McCAD::Decomposition::SolidSplitter::calculateHalfOBB(
        const TopoDS_Shape& halfSpace,
        const TopoDS_Shape& boundingBox) const{
    try{
        BRepAlgoAPI_Common intersection{halfSpace, boundingBox};
        if (intersection.IsDone()){
            return intersection.Shape();
        }
    }catch(...){
    }
    return std::nullopt;
}

std::optional<TopoDS_Shape>
McCAD::Decomposition::SolidSplitter::calculateHalfSolid(
        const TopoDS_Solid& solid,
        const TopoDS_Shape& boxToIntersect,
        const TopoDS_Shape& boxToSubtract) const{
    try{
        // Intersection of two shapes
        BRepAlgoAPI_Common common{solid, boxToIntersect};
        if(common.IsDone()){
            return common.Shape();
        }

        // Subtraction of second shape from first shape
        BRepAlgoAPI_Cut cutter{solid, boxToSubtract};
        if(cutter.IsDone()){
            return cutter.Shape();
        }
    }catch(...){
    }
    return std::nullopt;
}

std::optional<std::pair<TopoDS_Shape, TopoDS_Shape>>
McCAD::Decomposition::SolidSplitter::calculateHalfSolids(
        const TopoDS_Solid& solid,
        const ShapePair& halfBoundingBoxes) const{
    auto firstHalfSolid
            = calculateHalfSolid(solid, halfBoundingBoxes.first, halfBoundingBoxes.second);
    if(!firstHalfSolid) return std::nullopt;

    auto secondHalfSolid
            = calculateHalfSolid(solid, halfBoundingBoxes.second, halfBoundingBoxes.first);
    if(!secondHalfSolid) return std::nullopt;

    return std::make_pair(*firstHalfSolid, *secondHalfSolid);
}


