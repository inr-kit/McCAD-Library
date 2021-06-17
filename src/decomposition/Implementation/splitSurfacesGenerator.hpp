#ifndef SPLITSURFACEGENERATOR_HPP
#define SPLITSURFACEGENERATOR_HPP

// C++
#include <memory>
// McCAD
#include "edge_impl.hpp"
#include "boundSurface_impl.hpp"
// OCC
#include <TopoDS_Face.hxx>

namespace McCAD::Decomposition{
  class SplitSurfaceGenerator{
  public:
    SplitSurfaceGenerator();
    ~SplitSurfaceGenerator();

    void generatePlaneOnEdge(const TopoDS_Face& firstFace,
                             const TopoDS_Face& secondFace,
                             std::shared_ptr<Geometry::Edge> edge);
  };
}

#endif //SPLITSURFACEGENERATOR_HPP
