#ifndef SOLIDSPLITTER_HPP
#define SOLIDSPLITTER_HPP

// C++
#include <optional>
#include <utility>
// OCCT
#include <TopoDS_Shape.hxx>
#include <Bnd_OBB.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Face.hxx>
#include <gp_Pnt.hxx>

namespace McCAD::Decomposition{
  class SolidSplitter{
  public:
      SolidSplitter(const int & debugLevel);
      ~SolidSplitter();
      std::optional<std::pair<TopoDS_Shape, TopoDS_Shape>> operator()(
              const TopoDS_Solid& solidToSplit, const Bnd_OBB& obb,
              const TopoDS_Face& splittingFace) const;
      TopoDS_Shape calculateOBB(Bnd_OBB obb,
                                double clearanceTolerance = 0.4) const;
  private:
      int debugLevel{ 0 };
      using PointPair = std::pair<gp_Pnt, gp_Pnt>;
      using SolidPair = std::pair<TopoDS_Solid, TopoDS_Solid>;
      using ShapePair = std::pair<TopoDS_Shape, TopoDS_Shape>;

      PointPair calculatePoints(const TopoDS_Face& splittingFace) const;
      SolidPair calculateHalfSpaces(const TopoDS_Face& splittingFace) const;
      std::optional<TopoDS_Shape> calculateHalfOBB(
              const TopoDS_Shape& halfSpace, const TopoDS_Shape& boundingBox) const;
      std::optional<ShapePair> calculateHalfBB(
              const TopoDS_Face& splittingFace, const TopoDS_Shape& boundingBox) const;
      std::optional<TopoDS_Shape> calculateHalfSolid(
              const TopoDS_Solid& solidToSplit, const TopoDS_Shape& boxToIntersect,
              const TopoDS_Shape& boxToSubtract) const;
      std::optional<ShapePair> calculateHalfSolids(
              const TopoDS_Solid& solidToSplit, const ShapePair& halfBoundingBoxes) const;
  };
}

#endif //SOLIDSPLITTER_HPP
