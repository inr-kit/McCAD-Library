#ifndef BOUNDSURFACECONE_IMPL_HPP
#define BOUNDSURFACECONE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "boundSurfaceCone.hpp"

namespace McCAD::Geometry{
    class BoundSurfaceCone::Impl {
        public:
            Impl(BoundSurfaceCone* backReference);
            ~Impl();

          BoundSurfaceCone* boundSurfaceCone;
          std::string surfaceType = "Cone";
          void generateExtendedCone(const double& boxDiagonalLength,
                                    const double& edgeTolerance);
    };
}

#endif //BOUNDSURFACECONE_IMPL_HPP
