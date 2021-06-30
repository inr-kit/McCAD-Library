#ifndef SPLITSURFACEGENERATOR_HPP
#define SPLITSURFACEGENERATOR_HPP

// C++
#include <memory>
#include <optional>
// McCAD
#include "edge_impl.hpp"
#include "boundSurface_impl.hpp"
// OCC
#include <Standard.hxx>
#include <TopoDS_Face.hxx>

namespace McCAD::Decomposition{
  class SplitSurfaceGenerator{
  public:
      SplitSurfaceGenerator();
      SplitSurfaceGenerator(const Standard_Real& edgeTolerance, const Standard_Real& precision);
      ~SplitSurfaceGenerator();
  private:
      Standard_Real edgeTolerance{1.0e-7};
      Standard_Real precision{1.0e-7};
  public:
      std::optional<TopoDS_Face> generatePlaneOnLine(
              const TopoDS_Face& firstFace, const TopoDS_Face& secondFace,
              const std::shared_ptr<Geometry::Edge>& edge);
      std::optional<TopoDS_Face> generatePlaneOnCurve(
              const std::shared_ptr<Geometry::Edge>& edge);
      std::optional<TopoDS_Face> generateSurfOnBSpline(
              const TopoDS_Face& firstFace, const TopoDS_Face& secondFace,
              const std::shared_ptr<Geometry::Edge>& edge);
      std::optional<TopoDS_Face> generatePlaneOn2Lines(
              const std::shared_ptr<Geometry::Edge>& firstEedge,
              const std::shared_ptr<Geometry::Edge>& secondEdge);
  };
}

#endif //SPLITSURFACEGENERATOR_HPP
