#ifndef SOLIDDECOMPOSER_HPP
#define SOLIDDECOMPOSER_HPP

// McCAD
#include "boundSurface_impl.hpp"
// OCCT
#include <TopoDS_Solid.hxx>
#include <Bnd_OBB.hxx>
#include <TopTools_HSequenceOfShape.hxx>

namespace McCAD::Decomposition{
  class SolidDecomposer{
  public:
      SolidDecomposer(const int & debugLevel);
      ~SolidDecomposer();

      bool operator()(const TopoDS_Solid& solid, const Bnd_OBB& obb,
                      const Geometry::BoundSurface& surface,
                      TopTools_HSequenceOfShape& subSolidsList) const;
  private:
      int debugLevel{ 0 };
      bool filterAndRepair(TopTools_HSequenceOfShape& subSolidsList,
                           double tolerance = 1.0e-4) const;
      TopTools_HSequenceOfShape gatherSubSolids(TopTools_HSequenceOfShape& solids,
                                                double tolerance = 1.0e-4) const;
  };
}

#endif //SOLIDDECOMPOSER_HPP
