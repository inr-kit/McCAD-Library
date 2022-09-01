//C++
#include <array>
#include <filesystem>
//McCAD
#include "boundSurfacePlane_impl.hpp"
#include "surface_impl.hpp"
#include "extendedFaceCreator.hpp"
//OCCT
#include <TopoDS_Face.hxx>
#include <Geom_Surface.hxx>
#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include "STEPControl_Writer.hxx"

McCAD::Geometry::BoundSurfacePlane::Impl::Impl(
        McCAD::Geometry::BoundSurfacePlane* backReference)
    : boundSurfacePlane{backReference}{
}

McCAD::Geometry::BoundSurfacePlane::Impl::~Impl(){
}

void
McCAD::Geometry::BoundSurfacePlane::Impl::generateExtendedPlane(
        const double& boxDiagonalLength, const double& edgeTolerance){
    TopoDS_Face face = boundSurfacePlane->accessSImpl()->face;
    boundSurfacePlane->accessSImpl()->extendedFace =
            Tools::ExtendedFaceCreator{edgeTolerance}.generateExtendedPlane(face,
                                                                            boxDiagonalLength);
}

