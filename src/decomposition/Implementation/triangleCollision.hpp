#ifndef TRIANGLECOLLISION_HPP
#define TRIANGLECOLLISION_HPP

//McCAD
#include "boundSurface_impl.hpp"
#include "meshtriangle_impl.hpp"

namespace McCAD::Decomposition{
    class TriangleCollision{
    public:
      TriangleCollision();
      TriangleCollision(const double& precision,
                        const double& distanceTolerance);
      ~TriangleCollision();
    private:
      double precision{1.0e-6}, distanceTolerance{1.0e-6};
    public:
      bool operator()(const McCAD::Geometry::BoundSurface& iFace,
                      const McCAD::Geometry::MeshTriangle& aTriangle,
                      int& aSide);
    private:
      bool triangleCollisionPlane(
              const McCAD::Geometry::BoundSurface& iFace,
              const McCAD::Geometry::MeshTriangle& aTriangle,
              int& aSide);
      bool triangleCollisionCyl(
              const McCAD::Geometry::BoundSurface& iFace,
              const McCAD::Geometry::MeshTriangle& aTriangle,
              int& aSide);
      bool triangleCollisionCone(
          const McCAD::Geometry::BoundSurface& iFace,
          const McCAD::Geometry::MeshTriangle& aTriangle,
          int& aSide);
    };
}

#endif //TRIANGLECOLLISION_HPP
