// McCAD
#include "surface_impl.hpp"
#include "ShapeView.hpp"
// OCC
#include <TopoDS_Wire.hxx>
#include <BRepTools.hxx>

void
McCAD::Geometry::Surface::Impl::initiate(const TopoDS_Face& aFace){
  face = aFace;
  countInternalLoops();
}

void
McCAD::Geometry::Surface::Impl::countInternalLoops(){
    Standard_Integer numLoops{0};
    for (const auto& tempWire : detail::ShapeView<TopAbs_WIRE>{face}){
        ++numLoops;
        TopoDS_Wire wire = TopoDS::Wire(tempWire);
        BRepTools::Update(wire);
    }
    internalLoops = numLoops;
}

