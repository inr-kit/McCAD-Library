#ifndef TRIANGLECOLLISION_HPP
#define TRIANGLECOLLISION_HPP

//McCAD
#include "boundSurface_impl.hpp"
#include "meshtriangle_impl.hpp"
//OCC
#include <Standard.hxx>

namespace McCAD::Decomposition{
    class TriangleCollision{
    public:
      TriangleCollision();
      TriangleCollision(const Standard_Real& precision,
                        const Standard_Real& distanceTolerance);
      ~TriangleCollision();
    private:
      Standard_Real precision{1.0e-7}, distanceTolerance{1.0e-5};
    public:
      Standard_Boolean operator()(const McCAD::Geometry::BoundSurface& iFace,
                                  const McCAD::Geometry::MeshTriangle& aTriangle,
                                  Standard_Integer& aSide);
    private:
      Standard_Boolean triangleCollisionPlane(
              const McCAD::Geometry::BoundSurface& iFace,
              const McCAD::Geometry::MeshTriangle& aTriangle,
              Standard_Integer& aSide);
      Standard_Boolean triangleCollisionCyl(
              const McCAD::Geometry::BoundSurface& iFace,
              const McCAD::Geometry::MeshTriangle& aTriangle,
              Standard_Integer& aSide);
    };
}

#endif //TRIANGLECOLLISION_HPP
