#ifndef FACECOLLISION_HPP
#define FACECOLLISION_HPP

//C++
#include <memory>
#include <vector>
//McCAD
#include "boundSurface_impl.hpp"
#include "meshtriangle_impl.hpp"
//OCC
#include <Standard.hxx>

namespace McCAD::Decomposition{
    class FaceCollision{
    public:
        FaceCollision();
        FaceCollision(const Standard_Real& precision,
                      const Standard_Real& distanceTolerance);
        ~FaceCollision();
    private:
        Standard_Real precision{1.0e-7}, distanceTolerance{1.0e-5};
    public:
      Standard_Boolean operator()(const McCAD::Geometry::BoundSurface& firstFace,
                                  const McCAD::Geometry::BoundSurface& secondFace,
                                  Standard_Integer& aSide);
      Standard_Boolean faceCollision(
              const McCAD::Geometry::BoundSurface& firstFace,
              const std::vector<std::shared_ptr<McCAD::Geometry::MeshTriangle>>& meshTriangleList,
              Standard_Integer& aSide);
    };
}

#endif //FACECOLLISION_HPP
