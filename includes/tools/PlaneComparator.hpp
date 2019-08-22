#ifndef SURFACECOMPARISON_HPP
#define SURFACECOMPARISON_HPP

// OCC
#include <TopoDS_Face.hxx>
#include <gp_Pln.hxx>
#include <Standard_ConstructionError.hxx>
#include <Standard_NoSuchObject.hxx>
#include <Standard.hxx>
// C++
#include <cmath>

namespace McCAD::Tools{

    class PlaneComparator{

    public:
        PlaneComparator(Standard_Real parameterTolerance
                        = 1.0e-7,
                        Standard_Real angularTolerance
                        = 1.0e-3 * M_PI,
                        Standard_Real distanceTolerance
                        = 1.0e-5);

        bool operator()(const TopoDS_Face& first,
                        const TopoDS_Face& second) const;

    private:
        Standard_Real parameterTolerance;
        Standard_Real angularTolerance;
        Standard_Real distanceTolerance;

        std::array<Standard_Real, 4> planeParameters(
                const gp_Pln& plane) const;

        bool equivalentPlaneParameters(
                const gp_Pln& first,
                const gp_Pln& second) const;

    };
}

#endif
