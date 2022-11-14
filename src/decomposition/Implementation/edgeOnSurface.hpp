#ifndef EDGEONSURFACE_HPP
#define EDGEONSURFACE_HPP

//C++
#include <memory>
//McCAD
#include <Standard.hxx>
#include "edge_impl.hpp"
//OCC
#include <TopoDS_Face.hxx>

namespace McCAD::Decomposition{
    class EdgeOnSurface{
    public:
      EdgeOnSurface() = default;

      bool operator()(const TopoDS_Face& face,
                      const McCAD::Geometry::Edge& aEdge,
                      double distanceTolerance = 1.0e-6);
    };
}

#endif //EDGEONSURFACE_HPP
