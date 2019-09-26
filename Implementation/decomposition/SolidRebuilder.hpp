#ifndef SOLIDREBUILDER_HPP
#define SOLIDREBUILDER_HPP

// C++
#include <optional>
#include <vector>

// OCC
#include <TopoDS_Face.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>

namespace McCAD::Decomposition{

    class SolidRebuilder{

    public:
        SolidRebuilder(
                Standard_Real faceMassTolerance = 1.0e-4,
                Standard_Real sewingTolerance = 1.0e-2,
                Standard_Real fixingTolerance = 1.0e-2);

        std::optional<TopoDS_Solid> operator()(
                const TopoDS_Solid& originalSolid) const;

    private:
        Standard_Real faceMassTolerance;
        Standard_Real sewingTolerance;
        Standard_Real fixingTolerance;

        // uses faceMassTolerance
        std::vector<const TopoDS_Face*> solidToFaces(
                const TopoDS_Solid& solid) const;

        // uses sewingTolerance
        TopoDS_Shape facesToShape(
                const std::vector<const TopoDS_Face*>& faces) const;

        std::optional<TopoDS_Shell> shapeToSingleShell(
                const TopoDS_Shape& shape) const;

        // uses fixingTolerance
        std::optional<TopoDS_Solid> shellToSolid(
                const TopoDS_Shell& shell) const;

    };

}

#endif
