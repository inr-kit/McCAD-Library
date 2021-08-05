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
#include <gp_Cylinder.hxx>

namespace McCAD::Decomposition{
  class SplitSurfaceGenerator{
  public:
      SplitSurfaceGenerator();
      SplitSurfaceGenerator(const Standard_Real& edgeTolerance,
                            const Standard_Real& precision,
                            const Standard_Real& angularTolerance);
      ~SplitSurfaceGenerator();
  private:
      Standard_Real edgeTolerance{1.0e-7}, precision{1.0e-7},
                    angularTolerance{1.0e-4 * M_PI};
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
              const std::shared_ptr<Geometry::Edge>& firstEdge,
              const std::shared_ptr<Geometry::Edge>& secondEdge);
      std::optional<TopoDS_Face> generatePlaneOnLineAxis(
              const gp_Cylinder& cylinder,
              const std::shared_ptr<Geometry::Edge>& edge);
  };
}

#endif //SPLITSURFACEGENERATOR_HPP
