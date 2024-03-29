// C++
#include <array>
// McCAD
#include "boundSurfaceCyl_impl.hpp"
// OCC
#include <Geom_Surface.hxx>
#include <BRepTools.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRep_Tool.hxx>

McCAD::Geometry::BoundSurfaceCyl::Impl::Impl(McCAD::Geometry::BoundSurfaceCyl* backReference)
    : boundSurfaceCyl{backReference}{
}

McCAD::Geometry::BoundSurfaceCyl::Impl::~Impl(){
}

void
McCAD::Geometry::BoundSurfaceCyl::Impl::generateExtendedCyl(
        const double& boxDiagonalLength, double tolerance){
  TopoDS_Face face = boundSurfaceCyl->accessSImpl()->face;
  Handle_Geom_Surface surface = BRep_Tool::Surface(face);
  std::array<double, 4> uvParameters;
  // UV parameters in class grom_Cylindrical_Surface: U1 = 0 and U2 = 2*PI.
  BRepTools::UVBounds(face, uvParameters[0], uvParameters[1], uvParameters[2],
          uvParameters[3]);

  rangeRadian = std::abs(uvParameters[1] - uvParameters[0]);
  std::array<double, 4> newUVParameters;
  newUVParameters[0] = 0;
  newUVParameters[1] = 2*M_PI;
  newUVParameters[2] = uvParameters[2] - boxDiagonalLength;
  newUVParameters[3] = uvParameters[3] + boxDiagonalLength;

  boundSurfaceCyl->accessSImpl()->extendedFace =
    BRepBuilderAPI_MakeFace(surface, newUVParameters[0], newUVParameters[1],
          newUVParameters[2], newUVParameters[3], tolerance);
}
