#ifndef SPLITSOLID_IMPL_HPP
#define SPLITSOLID_IMPL_HPP

// C++
#include <array>
#include <filesystem>
#include <optional>
// McCAD
#include "splitsolid.hpp"
#include "boundSurface_impl.hpp"
#include "ShapeView.hpp"
// OCC
#include <TopTools_HSequenceOfShape.hxx>
#include <BRepBndLib.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeHalfSpace.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <gp_Pln.hxx>
#include <gp_Ax3.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <STEPControl_Writer.hxx>
#include <GProp_GProps.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <ShapeFix_Solid.hxx>
#include <TopoDS_Shell.hxx>

namespace McCAD::Decomposition{
    class SplitSolid::Impl {
    public:
        Impl() = default;

        bool operator()(
                const TopoDS_Solid& solid,
                const Geometry::BoundSurface& surface,
                TopTools_HSequenceOfShape& subSolidsList) const;

    private:
        bool filterAndRepair(
                TopTools_HSequenceOfShape& subSolidsList,
                Standard_Real tolerance = 1.0e-4) const;

        TopTools_HSequenceOfShape gatherSubSolids(
                TopTools_HSequenceOfShape& solids,
                Standard_Real tolerance = 1.0e-4) const;

    };
}

#endif //SPLITSOLID_IMPL_HPP
