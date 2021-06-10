#ifndef SPLITSOLID_IMPL_HPP
#define SPLITSOLID_IMPL_HPP

// McCAD
#include "splitsolid.hpp"
#include "boundSurface_impl.hpp"
// OCC
#include <Standard.hxx>
#include <TopoDS_Solid.hxx>
#include <Bnd_OBB.hxx>
#include <TopTools_HSequenceOfShape.hxx>

namespace McCAD::Decomposition{
    class SplitSolid::Impl {
    public:
        Impl() = default;

        Standard_Boolean operator()(const TopoDS_Solid& solid,
                                    const Bnd_OBB& obb,
                                    const Geometry::BoundSurface& surface,
                                    TopTools_HSequenceOfShape& subSolidsList) const;
    private:
        Standard_Boolean filterAndRepair(TopTools_HSequenceOfShape& subSolidsList,
                                         Standard_Real tolerance = 1.0e-4) const;
        TopTools_HSequenceOfShape gatherSubSolids(TopTools_HSequenceOfShape& solids,
                                                  Standard_Real tolerance = 1.0e-4) const;
    };
}

#endif //SPLITSOLID_IMPL_HPP
