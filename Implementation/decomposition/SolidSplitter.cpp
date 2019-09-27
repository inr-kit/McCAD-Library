#include "SolidSplitter.hpp"

// C++
#include <array>

// OCC
#include <Bnd_Box.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepBndLib.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeHalfSpace.hxx>
#include <gp_Pln.hxx>

std::optional<std::pair<TopoDS_Shape, TopoDS_Shape>>
McCAD::Decomposition::SolidSplitter::operator()(
        const TopoDS_Solid& solidToSplit,
        const TopoDS_Face& splittingFace) const{
    auto boundingBox = calculateBoundingBox(solidToSplit);

    auto halfBoundingBoxes = calculateHalfBoundingBoxes(splittingFace, boundingBox);
    if(!halfBoundingBoxes) return std::nullopt;

    return calculateHalfSolids(solidToSplit, *halfBoundingBoxes);
}

TopoDS_Shape
McCAD::Decomposition::SolidSplitter::calculateBoundingBox(
        const TopoDS_Solid& solid) const{
    Bnd_Box boundingBox;
    BRepBndLib::Add(solid, boundingBox);
    boundingBox.SetGap(0.5);

    std::array<Standard_Real, 3> minCoords;
    std::array<Standard_Real, 3> maxCoords;
    boundingBox.Get(minCoords[0], minCoords[1], minCoords[2],
                    maxCoords[0], maxCoords[1], maxCoords[2]);
    return BRepPrimAPI_MakeBox{
        gp_Pnt{minCoords[0], minCoords[1], minCoords[2]},
        gp_Pnt{maxCoords[0], maxCoords[1], maxCoords[2]}
    }.Shape();
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
McCAD::Decomposition::SolidSplitter::calculateHalfBoundingBoxes(
        const TopoDS_Face& splittingFace,
        const TopoDS_Shape& boundingBox) const{
    auto halfSpaces = calculateHalfSpaces(splittingFace);

    auto firstHalfBB = calculateHalfBoundingBox(halfSpaces.first, boundingBox);
    if(!firstHalfBB) return std::nullopt;

    auto secondHalfBB = calculateHalfBoundingBox(halfSpaces.second, boundingBox);
    if(!secondHalfBB) return std::nullopt;

    return std::make_pair(*firstHalfBB, *secondHalfBB);
}

std::optional<TopoDS_Shape>
McCAD::Decomposition::SolidSplitter::calculateHalfBoundingBox(
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


