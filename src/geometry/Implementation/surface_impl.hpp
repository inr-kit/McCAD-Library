#ifndef SURFACE_IMPL_HPP
#define SURFACE_IMPL_HPP

// C++
#include <string>
#include <vector>
// McCAD
#include "surface.hpp"
#include "tools_impl.hpp"
// OCC
#include <Standard.hxx>
#include <TopoDS_Face.hxx>
#include <gp_Pln.hxx>
#include <gp_Cylinder.hxx>
#include <gp_Torus.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

namespace McCAD::Geometry{
  class Surface::Impl {
  public:
    Impl() = default;

    TopoDS_Face face;
    TopoDS_Face extendedFace;
    std::string surfaceType;
    Standard_Integer surfaceNumber;
    Standard_Boolean splitSurface = Standard_False;
    Standard_Boolean hasAssistSurface = Standard_False;
    Standard_Boolean isAssistSurface = Standard_False;
    Standard_Integer numberCollidingSurfaces = 0;
    Standard_Integer numberCollidingCurvedSurfaces = 0;
    Standard_Integer throughConcaveEdges = 0;
    Standard_Integer internalLoops = 0;
    Standard_Integer repeatedSurface = 0;

    void initiate(const TopoDS_Face& aFace);
    void countInternalLoops();

    // Conversion variables.
    Standard_Boolean updated{Standard_False};
    std::string surfSymb;
    Standard_Integer uniqueID;
    std::string surfExpr;
    // Shared between surfaces.
    signed int surfSense;
    gp_Pnt location;
    gp_Dir symmetryAxis;
    std::vector<Standard_Real> surfParameters;
    // Plane
    gp_Pln plane;
    gp_Dir normal;
    // Cylinder
    gp_Cylinder cylinder;
    Standard_Real radius;
    // Torus
    gp_Torus torus;
    Standard_Real minorRadius, majorRadius;
  };
}

#endif //SURFACE_IMPL_HPP
