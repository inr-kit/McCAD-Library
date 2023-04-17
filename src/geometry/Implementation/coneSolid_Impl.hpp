#pragma once
#ifndef CONESOLID_IMPL_HPP
#define CONESOLID_IMPL_HPP

// McCAD
#include "coneSolid.hpp"
#include "solid_impl.hpp"
// OCC
#include "Standard.hxx"

namespace McCAD::Geometry {
    class CONESolid::Impl {       
    public:
        Impl() = default;

        void judgeDecomposeSurfaces(Solid::Impl*& solidImpl,
            Standard_Real precision = 1.0e-6,
            Standard_Real distanceTolerance = 1.0e-6);
        void judgeThroughConcaveEdges(Solid::Impl*& solidImpl);
        void judgeAssistDecomposeSurfaces(Solid::Impl* solidImpl,
            Standard_Real precision = 1.0e-6,
            Standard_Real distanceTolerance = 1.0e-6);
    };
}

#endif //CONESOLID_IMPL_HPP
