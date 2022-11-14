#ifndef SURFACECOMPARATOR_HPP
#define SURFACECOMPARATOR_HPP

// OCCT
#include <TopoDS_Face.hxx>

namespace McCAD::Tools{
  class SurfaceComparator{
  public:
      SurfaceComparator();
      SurfaceComparator(const double& precision, const double& angularTolerance,
                        const double& distanceTolerance);
      ~SurfaceComparator();
  private:
      double precision{1.0e-6};
      double angularTolerance{1.0e-4 * M_PI};
      double distanceTolerance{1.0e-6};
  public:
      bool operator()(const TopoDS_Face& firstFace, const TopoDS_Face& secondFace) const;
  };
}

#endif //SURFACECOMPARATOR_HPP
