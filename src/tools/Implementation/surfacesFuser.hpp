#ifndef SURFACESFUSER_HPP
#define SURFACESFUSER_HPP

// C++
#include <array>
#include <optional>
// OCC
#include <TopoDS_Face.hxx>
#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <GeomAbs_SurfaceType.hxx>

namespace McCAD::Tools{
    class SurfacesFuser{
    public:
        SurfacesFuser(Standard_Real zeroTolerance = 1.0e-7,
                      Standard_Real tolerance = 1.0e-7);

        std::optional<TopoDS_Face> operator()(const TopoDS_Face& firstFace,
                                              const TopoDS_Face& secondFace);
    private:
        Standard_Real zeroTolerance;
        Standard_Real tolerance;

        std::array<Standard_Real, 4> uvBounds(const TopoDS_Face& face) const;
        TopoDS_Face fusePlanes(const TopoDS_Face& first,
                               const TopoDS_Face& second);
        TopoDS_Face fuseCyls(const TopoDS_Face& first,
                             const TopoDS_Face& second);
    };
}

#endif //SURFACESFUSER_HPP
