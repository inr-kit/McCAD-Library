#ifndef SURFACECOMPARATOR_HPP
#define SURFACECOMPARATOR_HPP

// C++
#include <cmath>
#include <array>
#include <optional>
// McCAD
#include <Standard.hxx>
// OCC
#include <TopoDS_Face.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <BRep_Tool.hxx>

namespace McCAD::Tools{
  class SurfaceComparator{
  public:
      SurfaceComparator() = default;
      Standard_Boolean operator()(const TopoDS_Face& firstFace,
                                  const TopoDS_Face& secondFace) const;
  };
}

#endif //SURFACECOMPARATOR_HPP
