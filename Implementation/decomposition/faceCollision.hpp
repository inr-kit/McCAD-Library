#ifndef FACECOLLISION_HPP
#define FACECOLLISION_HPP

//C++
#include <memory>
//McCAD
#include "boundSurface_impl.hpp"
#include "meshtriangle_impl.hpp"
#include <Standard.hxx>
//OCC

namespace McCAD::Decomposition{
    class FaceCollision{
    public:
      FaceCollision() = default;

      Standard_Boolean operator()(const McCAD::Geometry::BoundSurface& iFace,
                                  const McCAD::Geometry::BoundSurface& jFace,
                                  Standard_Integer& aSide);
      Standard_Boolean faceCollisionPlane(
              const TopoDS_Face& face,
              const std::vector<std::unique_ptr<McCAD::Geometry::MeshTriangle>>& meshTriangleList,
              Standard_Integer& aSide);
    };
}

#endif //FACECOLLISION_HPP
