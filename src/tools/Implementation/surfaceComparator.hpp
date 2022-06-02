#ifndef SURFACECOMPARATOR_HPP
#define SURFACECOMPARATOR_HPP

// OCC
#include <Standard.hxx>
#include <TopoDS_Face.hxx>

namespace McCAD::Tools{
  class SurfaceComparator{
  public:
      SurfaceComparator();
      SurfaceComparator(const Standard_Real& precision, const Standard_Real& angularTolerance,
                        const Standard_Real& distanceTolerance);
      ~SurfaceComparator();
  private:
      Standard_Real precision{1.0e-6};
      Standard_Real angularTolerance{1.0e-4 * M_PI};
      Standard_Real distanceTolerance{1.0e-6};
  public:
      Standard_Boolean operator()(const TopoDS_Face& firstFace,
                                  const TopoDS_Face& secondFace) const;
  };
}

#endif //SURFACECOMPARATOR_HPP
