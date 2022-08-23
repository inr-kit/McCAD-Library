//C++
#include <array>
#include <filesystem>
//McCAD
#include "boundSurfaceCone_impl.hpp"
#include "surface_impl.hpp"
#include "extendedFaceCreator.hpp"
//OCC
#include <TopoDS_Face.hxx>
#include <Geom_Surface.hxx>
#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include "STEPControl_Writer.hxx"

McCAD::Geometry::BoundSurfaceCone::Impl::Impl(
        McCAD::Geometry::BoundSurfaceCone* backReference)
    : boundSurfaceCone{backReference}{
}

McCAD::Geometry::BoundSurfaceCone::Impl::~Impl(){
}

/** ********************************************************************
* @brief    A function that calls the extended surface creator on the current face.
* @param    boxDiagonalLength is a calculated diagonal length of the solid OBB.
* @param    edgeTolerance is set on the inputComfig file.
* @date     23/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
void
McCAD::Geometry::BoundSurfaceCone::Impl::generateExtendedCone(
        const double& boxDiagonalLength,
        const double& edgeTolerance){
    TopoDS_Face face = boundSurfaceCone->accessSImpl()->face;
    boundSurfaceCone->accessSImpl()->extendedFace =
            Tools::ExtendedFaceCreator{edgeTolerance}.generateExtendedCone(face,
                                                                           boxDiagonalLength);
}

