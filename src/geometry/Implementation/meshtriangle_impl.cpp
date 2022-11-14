// McCAD
#include "meshtriangle_impl.hpp"
// OCC
#include <BRepBndLib.hxx>

void
McCAD::Geometry::MeshTriangle::Impl::initiate(const TopoDS_Face& aFace){
    face = aFace;
    createOBB(face);
}

void
McCAD::Geometry::MeshTriangle::Impl::createOBB(const TopoDS_Face& aFace,
                                               double bndBoxGap){
    BRepBndLib::AddOBB(aFace, obb);
    //obb.Enlarge(bndBoxGap);
}
