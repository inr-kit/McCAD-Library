#ifndef SOLIDSPLITTER_HPP
#define SOLIDSPLITTER_HPP

// C++
#include <optional>
#include <utility>

// OCC
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Solid.hxx>
#include <gp_Pnt.hxx>

namespace McCAD::Decomposition{

    class SolidSplitter{

    public:
        std::optional<std::pair<TopoDS_Shape, TopoDS_Shape>> operator()(
                const TopoDS_Solid& solidToSplit,
                const TopoDS_Face& splittingFace) const;

    private:
        using PointPair = std::pair<gp_Pnt, gp_Pnt>;
        using SolidPair = std::pair<TopoDS_Solid, TopoDS_Solid>;
        using ShapePair = std::pair<TopoDS_Shape, TopoDS_Shape>;

        TopoDS_Shape calculateBoundingBox(
                const TopoDS_Solid& solidToSplit) const;

        PointPair calculatePoints(
                const TopoDS_Face& splittingFace) const;

        SolidPair calculateHalfSpaces(
                const TopoDS_Face& splittingFace) const;

        std::optional<TopoDS_Shape> calculateHalfBoundingBox(
                const TopoDS_Shape& halfSpace,
                const TopoDS_Shape& boundingBox) const;

        std::optional<ShapePair> calculateHalfBoundingBoxes(
                const TopoDS_Face& splittingFace,
                const TopoDS_Shape& boundingBox) const;

        std::optional<TopoDS_Shape> calculateHalfSolid(
                const TopoDS_Solid& solidToSplit,
                const TopoDS_Shape& boxToIntersect,
                const TopoDS_Shape& boxToSubtract) const;

        std::optional<ShapePair> calculateHalfSolids(
                const TopoDS_Solid& solidToSplit,
                const ShapePair& halfBoundingBoxes) const;

    };

}

#endif
