#ifndef FACECOLLISION_HPP
#define FACECOLLISION_HPP

//C++
#include <memory>
#include <vector>
//McCAD
#include "boundSurface_impl.hpp"
#include "meshtriangle_impl.hpp"

namespace McCAD::Decomposition{
    class FaceCollision{
    public:
        FaceCollision();
        FaceCollision(const double& precision,
                      const double& distanceTolerance);
        ~FaceCollision();
    private:
        double precision{1.0e-6}, distanceTolerance{1.0e-6};
    public:
      bool operator()(const McCAD::Geometry::BoundSurface& firstFace,
                      const McCAD::Geometry::BoundSurface& secondFace,
                      int& aSide);
      bool faceCollision(
              const McCAD::Geometry::BoundSurface& firstFace,
              const std::vector<std::shared_ptr<McCAD::Geometry::MeshTriangle>>& meshTriangleList,
              int& aSide);
    };
}

#endif //FACECOLLISION_HPP
