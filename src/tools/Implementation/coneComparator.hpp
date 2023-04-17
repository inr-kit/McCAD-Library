#pragma once
#ifndef CONECOMPARATOR_HPP
#define CONECOMPARATOR_HPP

// C++
#include <array>
#include <optional>
// OCC
#include <Standard.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <gp_Cone.hxx>

namespace McCAD::Tools {
    class ConeComparator {
    public:
        ConeComparator();
        ConeComparator(const Standard_Real& precision, const Standard_Real& angularTolerance,
            const Standard_Real& distanceTolerance);
        ~ConeComparator();
    private:
        Standard_Real precision{ 1.0e-6 };
        Standard_Real angularTolerance{ 1.0e-4 * M_PI };
        Standard_Real distanceTolerance{ 1.0e-6 };
    public:
        std::optional<Standard_Boolean> operator()(const GeomAdaptor_Surface& firstAdaptor,
            const GeomAdaptor_Surface& secondAdaptor);
        std::array<Standard_Real, 10> coneParameters(const gp_Cone& cone) const;
        std::optional<Standard_Boolean> equivalentConeParameters(const gp_Cone& first,
            const gp_Cone& second) const;
    };
}

#endif //CONECOMPARATOR_HPP
