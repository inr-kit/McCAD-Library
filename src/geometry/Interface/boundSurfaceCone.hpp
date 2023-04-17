#pragma once
#ifndef BOUNDSURFACECONE_HPP
#define BOUNDSURFACECONE_HPP

// C++
// McCAD
#include "boundSurface.hpp"
#include <Standard.hxx>

namespace McCAD::Geometry {
    class BoundSurfaceCone : public BoundSurface {
    private:
        class Impl;

    public:
        BoundSurfaceCone();
        ~BoundSurfaceCone();

        Impl* accessBSCOImpl() const;

    private:
        std::unique_ptr<Impl> pBSCOImpl;
    };
}
#endif //BOUNDSURFACECONE_HPP
