#ifndef BOUNDSURFACE_IMPL_HPP
#define BOUNDSURFACE_IMPL_HPP

// C++
// McCAD
#include "boundSurface.hpp"
#include "surface_impl.hpp"
//#include "mesh.hpp"
// OCC
#include <Handle_Poly_Triangulation.hxx>
#include <Poly_Triangulation.hxx>
#include <Handle_Geom_Surface.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS_Face.hxx>
#include <TColgp_Array1OfPnt.hxx>
//#include <gp_Pln.hxx>
//#include <gp_Ax3.hxx>
//#include <TopExp_Explorer.hxx>
//#include <TopAbs_ShapeEnum.hxx>
//#include <TopoDS_Wire.hxx>
//#include <TopoDS.hxx>
//#include <TopTools_HSequenceOfShape.hxx>
//#include <TCollection_AsciiString.hxx>
//#include <BRepTools.hxx>
//#include <BRepBndLib.hxx>
//#include <Bnd_Box.hxx>
//#include <BRepBuilderAPI_MakePolygon.hxx>
//#include <BRepBuilderAPI_MakeFace.hxx>
//#include <BRepAlgoAPI_Section.hxx>
//#include <Handle_TColgp_HSequenceOfPnt.hxx>
//#include <GeomAdaptor_Curve.hxx>

namespace McCAD::Decomposition{
  class BoundSurface::Impl {
  public:
    Impl(McCAD::Decomposition::BoundSurface* backReference);
    ~Impl();

    McCAD::Decomposition::BoundSurface* boundSurface;
    //std::vector<std::unique_ptr<McCAD::Decomposition::Mesh>> meshList;

    Standard_Boolean generateMesh(const Standard_Real& meshDeflection);

  private:

  };
}

#endif //BOUNDSURFACE_IMPL_HPP
