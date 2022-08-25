#ifndef SURFACESFUSER_HPP
#define SURFACESFUSER_HPP

// C++
#include <array>
#include <optional>
// OCCT
#include <TopoDS_Face.hxx>
#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <GeomAbs_SurfaceType.hxx>

namespace McCAD::Tools{
    class SurfacesFuser{
    public:
        SurfacesFuser(double precision = 1.0e-6,
                      double edgeTolerance = 1.0e-8);

        std::optional<TopoDS_Face> operator()(const TopoDS_Face& firstFace,
                                              const TopoDS_Face& secondFace);
    private:
        double precision, edgeTolerance;

        std::array<double, 4> uvBounds(const TopoDS_Face& face) const;
        TopoDS_Face fusePlanes(const TopoDS_Face& first,
                               const TopoDS_Face& second);
        TopoDS_Face fuseCyls(const TopoDS_Face& first,
                             const TopoDS_Face& second);
        TopoDS_Face fuseCones(const TopoDS_Face& first,
                              const TopoDS_Face& second);
    };
}

#endif //SURFACESFUSER_HPP
