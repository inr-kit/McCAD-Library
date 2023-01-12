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

namespace McCAD::Geometry{
  class BoundSurface::Impl{
  public:
    Impl(BoundSurface* backReference);
    ~Impl();

    BoundSurface* boundSurface;
    std::vector<std::shared_ptr<Edge>> edgesList;
    std::vector<std::shared_ptr<Edge>> assistEdgesList;
    std::vector<std::shared_ptr<MeshTriangle>> meshTrianglesList;

    bool isEqual(const BoundSurface& that);
    bool canFuse(const BoundSurface& that);
    bool faceCollision(const BoundSurface& aFace, int& aSide);
    bool generateMesh(const double& meshDeflection);
    void generateEdges(const double& parameterTolerance);
    void combineEdges(std::vector<std::shared_ptr<Edge>>& aEdgesList);
    bool generateParmts(double precision = 1.0e-6,
                        double scalingFactor = 1.0);
  };
}
#endif //BOUNDSURFACE_IMPL_HPP
