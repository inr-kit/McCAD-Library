#pragma once
#ifndef BOUNDSURFACECONE_IMPL_HPP
#define BOUNDSURFACECONE_IMPL_HPP

// C++
#include <string>
// McCAD
#include "boundSurfaceCone.hpp"
#include "surface_impl.hpp"
// OCC
#include <Standard.hxx>

namespace McCAD::Geometry {
    class BoundSurfaceCone::Impl {
    public:
        Impl(BoundSurfaceCone* backReference);
        ~Impl();

        BoundSurfaceCone* boundSurfaceCone;

        std::string surfaceType = "Cone";
        Standard_Real rangeRadian;
        void generateExtendedCone(const Standard_Real& boxDiagonalLength,
            Standard_Real degenerateEdgesTolerance = 1.0e-7);
    };
}

#endif //BOUNDSURFACE
