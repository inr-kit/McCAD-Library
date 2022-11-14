#ifndef SURFACESMERGER_HPP
#define SURFACESMERGER_HPP

//C++
#include <memory>
#include <vector>
//McCAD
#include "boundSurface_impl.hpp"

namespace McCAD::Decomposition{
    class SurfacesMerger{
    public:
        SurfacesMerger() = default;

        void operator()(std::vector<std::shared_ptr<Geometry::BoundSurface>>& surfacesList,
                        const double& boxDiagonalLength,
                        const double& precision,
                        const double& edgeTolerance,
                        const double& angularTolerance,
                        const double& distanceTolerance);
    };
}

#endif //SURFACESMERGER_HPP
