// McCAD
#include "surface_impl.hpp"
#include "ShapeView.hpp"
// OCC
#include <TopoDS_Wire.hxx>
#include <BRepTools.hxx>

/** ********************************************************************
* @brief    A function that assigns the face to this object.
* @param    aFace is a OCCT face.
* @date     23/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
void
McCAD::Geometry::Surface::Impl::initiate(const TopoDS_Face& aFace){
  face = aFace;
  countInternalLoops();
}

/** ********************************************************************
* @brief    A function that counts internal loops on a surface.
* @date     23/08/2022
* @modified
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
void
McCAD::Geometry::Surface::Impl::countInternalLoops(){
    int numLoops{0};
    for (const auto& tempWire : detail::ShapeView<TopAbs_WIRE>{face}){
        ++numLoops;
        TopoDS_Wire wire = TopoDS::Wire(tempWire);
        BRepTools::Update(wire);
    }
    internalLoops = numLoops;
}

