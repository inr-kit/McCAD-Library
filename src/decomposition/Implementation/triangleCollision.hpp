#ifndef TRIANGLECOLLISION_HPP
#define TRIANGLECOLLISION_HPP

//McCAD
#include "meshtriangle_impl.hpp"
#include "boundSurface_impl.hpp"
//OCC
#include <Standard.hxx>

namespace McCAD::Decomposition{
    class TriangleCollision{
    public:
      TriangleCollision() = default;

      Standard_Boolean operator()(const McCAD::Geometry::BoundSurface& iFace,
                                  const McCAD::Geometry::MeshTriangle& aTriangle,
                                  Standard_Integer& aSide);

    private:
      Standard_Boolean triangleCollisionPlane(
              const McCAD::Geometry::BoundSurface& iFace,
              const McCAD::Geometry::MeshTriangle& aTriangle,
              Standard_Integer& aSide,
              Standard_Real distanceTolerance = 1.0e-2,
              Standard_Real senseTolerance = 1.0e-3);
      Standard_Boolean triangleCollisionCyl(
              const McCAD::Geometry::BoundSurface& iFace,
              const McCAD::Geometry::MeshTriangle& aTriangle,
              Standard_Integer& aSide,
              Standard_Real distanceTolerance = 1.0e-2,
              Standard_Real senseTolerance = 1.0e-3);
    };
}

#endif //TRIANGLECOLLISION_HPP
