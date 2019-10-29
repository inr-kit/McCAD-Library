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

      template<typename surfaceType>
      Standard_Boolean edgeOnSurface(const TopoDS_Face& face,
                                     const McCAD::Geometry::Edge& aEdge,
                                     Standard_Real tolerance = 1.0e-4);
    };
}

#include "edgeOnSurface.tpp"

#endif //EDGEONSURFACE_HPP
