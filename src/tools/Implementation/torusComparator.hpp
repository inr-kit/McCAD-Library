#pragma once
#ifndef TORUSCOMPARATOR_HPP
#define TORUSCOMPARATOR_HPP

// C++
#include <array>
#include <optional>
// OCC
#include <Standard.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <gp_Torus.hxx>

namespace McCAD::Tools {
    class TorusComparator {
    public:
        TorusComparator();
        TorusComparator(const Standard_Real& precision, const Standard_Real& angularTolerance,
            const Standard_Real& distanceTolerance);
        ~TorusComparator();
    private:
        Standard_Real precision{ 1.0e-6 };
        Standard_Real angularTolerance{ 1.0e-4 * M_PI };
        Standard_Real distanceTolerance{ 1.0e-6 };
    public:
        std::optional<Standard_Boolean> operator()(const GeomAdaptor_Surface& firstAdaptor,
            const GeomAdaptor_Surface& secondAdaptor);
        std::array<Standard_Real, 35> torusParameters(const gp_Torus& torus) const;
        std::optional<Standard_Boolean> equivalentTorusParameters(const gp_Torus& first,
            const gp_Torus& second) const;
    };
}

#endif //TORUSCOMPARATOR_HPP
