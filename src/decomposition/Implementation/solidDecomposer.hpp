#ifndef SOLIDDECOMPOSER_HPP
#define SOLIDDECOMPOSER_HPP

// McCAD
#include "boundSurface_impl.hpp"
// OCC
#include <Standard.hxx>
#include <TopoDS_Solid.hxx>
#include <Bnd_OBB.hxx>
#include <TopTools_HSequenceOfShape.hxx>

namespace McCAD::Decomposition{
  class SolidDecomposer{
  public:
      SolidDecomposer();
      ~SolidDecomposer();

      Standard_Boolean operator()(const TopoDS_Solid& solid, const Bnd_OBB& obb,
                                  const Geometry::BoundSurface& surface,
                                  TopTools_HSequenceOfShape& subSolidsList) const;
  private:
      Standard_Boolean filterAndRepair(TopTools_HSequenceOfShape& subSolidsList,
                                       Standard_Real tolerance = 1.0e-4) const;
      TopTools_HSequenceOfShape gatherSubSolids(TopTools_HSequenceOfShape& solids,
                                                Standard_Real tolerance = 1.0e-4) const;
  };
}

#endif //SOLIDDECOMPOSER_HPP
