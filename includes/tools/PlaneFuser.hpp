#ifndef PLANEFUSER_HPP
#define PLANEFUSER_HPP

// OCC
#include <TopoDS_Face.hxx>

// C++
#include <array>

namespace McCAD::Tools{

    class PlaneFuser{

    public:
        PlaneFuser(Standard_Real zeroTolerance
                   = 1.0e-7,
                   Standard_Real tolerance
                   = 1.0e-7);

        TopoDS_Face operator()(const TopoDS_Face& first,
                               const TopoDS_Face& second) const;

    private:
        Standard_Real zeroTolerance;
        Standard_Real tolerance;

        std::array<Standard_Real, 4> uvBounds(
                const TopoDS_Face& face) const;

    };


}

#endif
