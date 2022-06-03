//C++
#include <array>
#include <filesystem>
//McCAD
#include "boundSurfacePlane_impl.hpp"
#include "surface_impl.hpp"
#include "extendedFaceCreator.hpp"
//OCC
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
        const Standard_Real& boxDiagonalLength,
        const Standard_Real& edgeTolerance){
    TopoDS_Face face = boundSurfacePlane->accessSImpl()->face;
    boundSurfacePlane->accessSImpl()->extendedFace =
            Tools::ExtendedFaceCreator{edgeTolerance}.generateExtendedPlane(face,
                                                                            boxDiagonalLength);
}

