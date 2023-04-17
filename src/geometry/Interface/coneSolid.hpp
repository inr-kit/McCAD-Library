#pragma once
#ifndef CONESOLID_HPP
#define CONESOLID_HPP

// C++
#include <memory>
// McCAD
#include <Standard.hxx>
#include "solid.hpp"
// OCC

namespace McCAD::Geometry {
    class CONESolid : public Solid {
    public:
        class Impl;

    public:
        CONESolid();
        ~CONESolid();

        Impl* accessCOSImpl() const;

    private:
        std::unique_ptr<Impl> pCOSImpl;
    };
}
#endif //CONESOLID_HPP
