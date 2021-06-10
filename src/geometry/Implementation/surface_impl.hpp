#ifndef SURFACE_IMPL_HPP
#define SURFACE_IMPL_HPP

// C++
#include <string>
#include <array>
// McCAD
#include "surface.hpp"
#include "tools_impl.hpp"
// OCC
#include <Standard.hxx>
#include <TopoDS_Face.hxx>
#include <gp_Pln.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

namespace McCAD::Geometry{
  class Surface::Impl {
  public:
    Impl() = default;

    TopoDS_Face face;
    TopoDS_Face extendedFace;
    TopoDS_Face AssistFace;
    std::string surfaceType;
    Standard_Integer surfaceNumber;
    Standard_Boolean splitSurface = Standard_False;
    Standard_Boolean hasAssistSurface = Standard_False;
    Standard_Integer numberCollidingSurfaces = 0;
    Standard_Integer numberCollidingCurvedSurfaces = 0;
    Standard_Integer throughConcaveEdges = 0;

    void initiate(const TopoDS_Face& aFace);

    // Conversion variables.
    Standard_Boolean updated{Standard_False};
    gp_Pln plane;
    gp_Pnt location;
    gp_Dir normal;
    std::array<Standard_Real, 4> surfParameters;
    std::string surfSymb;
    Standard_Integer uniqueID;
    std::string surfExpr;
    signed int surfSense;
  };
}

#endif //SURFACE_IMPL_HPP
