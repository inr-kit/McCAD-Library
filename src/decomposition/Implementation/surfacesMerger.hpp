#ifndef SURFACESMERGER_HPP
#define SURFACESMERGER_HPP

//C++
#include <memory>
#include <vector>
//McCAD
#include <Standard.hxx>
#include "boundSurface_impl.hpp"

namespace McCAD::Decomposition{
    class SurfacesMerger{
    public:
        SurfacesMerger() = default;

        void operator()(std::vector<std::shared_ptr<Geometry::BoundSurface>>& surfacesList,
                        const Standard_Real& boxDiagonalLength,
                        const Standard_Real& precision,
                        const Standard_Real& edgeTolerance,
                        const Standard_Real& angularTolerance,
                        const Standard_Real& distanceTolerance);
    };
}

#endif //SURFACESMERGER_HPP
