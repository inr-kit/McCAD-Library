#ifndef SURFACESMERGER_HPP
#define SURFACESMERGER_HPP

//C++
#include <memory>
//McCAD
#include <Standard.hxx>
#include "boundSurface_impl.hpp"
//OCC
#include <TopoDS_Face.hxx>

namespace McCAD::Decomposition{
    class SurfacesMerger{
    public:
        SurfacesMerger() = default;

        void operator()(std::vector<std::shared_ptr<Geometry::BoundSurface>>& surfacesList,
                        const Standard_Real& boxDiagonalLength);
    };
}

#endif //SURFACESMERGER_HPP
