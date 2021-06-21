#ifndef BOUNDSURFACE_IMPL_HPP
#define BOUNDSURFACE_IMPL_HPP

// C++
#include <vector>
#include <memory>
// McCAD
#include "boundSurface.hpp"
#include "surface_impl.hpp"
#include "meshtriangle_impl.hpp"
#include "edge_impl.hpp"
// OCC
#include <Standard.hxx>

namespace McCAD::Geometry{
  class BoundSurface::Impl{
  public:
    Impl(BoundSurface* backReference);
    ~Impl();

    BoundSurface* boundSurface;
    
    std::vector<std::shared_ptr<Edge>> edgesList;
    std::vector<std::shared_ptr<Edge>> assistEdgesList;
    std::vector<std::shared_ptr<MeshTriangle>> meshTrianglesList;

    Standard_Boolean isEqual(const BoundSurface& that);
    Standard_Boolean canFuse(const BoundSurface& that);
    Standard_Boolean faceCollision(const BoundSurface& aFace,
                                   Standard_Integer& aSide);
    Standard_Boolean generateMesh(const Standard_Real& meshDeflection);
    void generateEdges(const Standard_Real& parameterTolerance);
    void combineEdges(std::vector<std::shared_ptr<Edge>>& aEdgesList);
    bool generateParmts();
  };
}
#endif //BOUNDSURFACE_IMPL_HPP
