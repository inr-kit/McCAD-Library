// C++
#include <array>
#include <filesystem>
//McCAD
#include "SolidSplitter.hpp"
// OCCT
#include <gp_Ax3.hxx>
#include <gp_Trsf.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeHalfSpace.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <GeomAbs_SurfaceType.hxx>
#include <gp_Pln.hxx>
#include <STEPControl_Writer.hxx>

McCAD::Decomposition::SolidSplitter::SolidSplitter(const int& debugLevel) 
    : debugLevel{ debugLevel } {
}

McCAD::Decomposition::SolidSplitter::~SolidSplitter() {
}

/** ********************************************************************
* @brief    An operator that manages the splitting of a given solid.
* @param    solidToSplit is a OCCT solid.
* @param    obb is a OCCT oriented bounding box.
* @param    splittingFace is a OCCT face.
* @return   an optional pair of OCCT shapes.
* @date     31/12/2020
* @modified 31/08/2022
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
std::optional<std::pair<TopoDS_Shape, TopoDS_Shape>>
McCAD::Decomposition::SolidSplitter::operator()(
        const TopoDS_Solid& solidToSplit, const Bnd_OBB& obb,
        const TopoDS_Face& splittingFace) const{
    auto boundingBox = calculateOBB(obb);
    //debug
    if (debugLevel >= 2) {
        STEPControl_Writer writer;
        writer.Transfer(boundingBox, STEPControl_StepModelType::STEPControl_AsIs);
        writer.Transfer(solidToSplit, STEPControl_StepModelType::STEPControl_AsIs);
        writer.Transfer(splittingFace, STEPControl_StepModelType::STEPControl_AsIs);
        int kk = 0;
        std::string filename = "SolidSplitter_surface";
        std::string suffix = ".stp";
        while (std::filesystem::exists(filename + std::to_string(kk) + suffix)) {
            ++kk;
        }
        filename += std::to_string(kk);
        filename += suffix;
        writer.Write(filename.c_str());
    }
    //debug
    auto halfBoundingBoxes = calculateHalfBB(splittingFace, boundingBox);
    if(!halfBoundingBoxes) return std::nullopt;
    return calculateHalfSolids(solidToSplit, *halfBoundingBoxes);
}

/** ********************************************************************
* @brief    A function that makes a solid of the OBB.
* @param    obb is a OCCT oriented bounding box.
* @param    clearanceTolerance controles the tightness of the OBB.
* @return   a OCCT shape.
* @date     31/12/2020
* @modified 31/08/2022
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
TopoDS_Shape
McCAD::Decomposition::SolidSplitter::calculateOBB(Bnd_OBB obb,
                                                  double clearanceTolerance) const{
    obb.Enlarge(clearanceTolerance);
    std::array<gp_Pnt, 8> corners;
    obb.GetVertex(&corners[0]);
    gp_Trsf transformation;
    transformation.SetTransformation(gp_Ax3{gp_Pnt(0, 0, 0), obb.ZDirection(),
                                            obb.XDirection()});
    auto boundingBox = BRepPrimAPI_MakeBox{corners[0].Transformed(transformation),
            corners[7].Transformed(transformation)}.Shape();
    BRepBuilderAPI_Transform boxTransform{transformation.Inverted()};
    boxTransform.Perform(boundingBox);
    return boxTransform.ModifiedShape(boundingBox);
}

/** ********************************************************************
* @brief    A function that calculates two points on either side of a splitting plane.
* @param    splittingFace is a OCCT face.
* @return   a pair of OCCT points.
* @date     31/12/2020
* @modified 31/08/2022
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
std::pair<gp_Pnt, gp_Pnt>
McCAD::Decomposition::SolidSplitter::calculatePoints(
        const TopoDS_Face& splittingFace) const{
    auto surfaceAdaptor = BRepAdaptor_Surface{splittingFace, true}.Surface();
    if (surfaceAdaptor.GetType() == GeomAbs_Plane){
        const auto& position = surfaceAdaptor.Plane().Position();
        const auto& direction = position.Direction();
        const auto& location = position.Location();
        return {location.Translated(+100 * direction),
                    location.Translated(-100 * direction)
        };
    }
    return {};
}

/** ********************************************************************
* @brief    A function that creates half spaces using a given splitting surface.
* @param    splittingFace is a OCCT face.
* @return   a pair of solids.
* @date     31/12/2020
* @modified 31/08/2022
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
std::pair<TopoDS_Solid, TopoDS_Solid>
McCAD::Decomposition::SolidSplitter::calculateHalfSpaces(
        const TopoDS_Face& splittingFace) const{
    auto points = calculatePoints(splittingFace);
    return {
        BRepPrimAPI_MakeHalfSpace{splittingFace, points.first}.Solid(),
        BRepPrimAPI_MakeHalfSpace{splittingFace, points.second}.Solid()
    };
}

/** ********************************************************************
* @brief    A function that creates half bounding boxes.
* @param    splittingFace is a OCCT face.
* @param    boundingBox is a OCCT shape.
* @return   an optional pair of shapes.
* @date     31/12/2020
* @modified 31/08/2022
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
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

/** ********************************************************************
* @brief    A function that calculates two points on either side of a splitting plane.
* @param    halfSpace is a OCCT shape.
* @param    boundingBox is a OCCT shape.
* @return   an optional OCCT shape.
* @date     31/12/2020
* @modified 31/08/2022
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
std::optional<TopoDS_Shape>
McCAD::Decomposition::SolidSplitter::calculateHalfOBB(
        const TopoDS_Shape& halfSpace,
        const TopoDS_Shape& boundingBox) const{
    try{
        BRepAlgoAPI_Common intersection{halfSpace, boundingBox};
        if (intersection.IsDone()) return intersection.Shape();
    } catch(...){}
    return std::nullopt;
}

/** ********************************************************************
* @brief    A function that makes a solid of the OBB.
* @param    solid is a OCCT solid.
* @param    boxToIntersect is a OCCT shape.
* @param    boxToSubtract is a OCCT shape.
* @return   an optional OCCT shape.
* @date     31/12/2020
* @modified 31/08/2022
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
std::optional<TopoDS_Shape>
McCAD::Decomposition::SolidSplitter::calculateHalfSolid(
        const TopoDS_Solid& solid,
        const TopoDS_Shape& boxToIntersect,
        const TopoDS_Shape& boxToSubtract) const{
    try{
        // Intersection of two shapes
        BRepAlgoAPI_Common common{solid, boxToIntersect};
        if(common.IsDone()) return common.Shape();
        // Subtraction of second shape from first shape
        BRepAlgoAPI_Cut cutter{solid, boxToSubtract};
        if(cutter.IsDone()) return cutter.Shape();
    } catch(...){}
    return std::nullopt;
}

/** ********************************************************************
* @brief    A function that makes a solid of the OBB.
* @param    solid is a OCCT solid.
* @param    halfBoundingBoxes a pair of OCCT shapes.
* @return   an optional pair of OCCT shapes.
* @date     31/12/2020
* @modified 31/08/2022
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
std::optional<std::pair<TopoDS_Shape, TopoDS_Shape>>
McCAD::Decomposition::SolidSplitter::calculateHalfSolids(
        const TopoDS_Solid& solid, const ShapePair& halfBoundingBoxes) const{
    auto firstHalfSolid = calculateHalfSolid(
                solid, halfBoundingBoxes.first, halfBoundingBoxes.second);
    if(!firstHalfSolid) return std::nullopt;
    auto secondHalfSolid = calculateHalfSolid(
                solid, halfBoundingBoxes.second, halfBoundingBoxes.first);
    if(!secondHalfSolid) return std::nullopt;
    return std::make_pair(*firstHalfSolid, *secondHalfSolid);
}


