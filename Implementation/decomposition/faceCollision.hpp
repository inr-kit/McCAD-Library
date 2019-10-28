#ifndef FACECOLLISION_HPP
#define FACECOLLISION_HPP

//C++
//McCAD
#include "boundSurface_impl.hpp"
#include <Standard.hxx>
//OCC

namespace McCAD::Decomposition{
    class FaceCollision{
    public:
      FaceCollision();
      ~FaceCollision();

      Standard_Boolean operator()(const BoundSurface& aFace,
                                  Standard_Integer& aSide);
      Standard_Boolean faceCollisionPlane(const BoundSurface& aFace,
                                          Standard_Integer& aSide);
    };
}

#endif //FACECOLLISION_HPP
