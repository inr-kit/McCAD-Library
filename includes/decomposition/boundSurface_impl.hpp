#ifndef BOUNDSURFACE_IMPL_HPP
#define BOUNDSURFACE_IMPL_HPP

// C++
// McCAD
#include "boundSurface.hpp"
#include "mesh.hpp"
// OCC
#include <Handle_Poly_Triangulation.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <gp_Pln.hxx>
#include <gp_Ax3.hxx>
#include <BRepMesh.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS_Face.hxx>
//#include <TopExp_Explorer.hxx>
//#include <TopAbs_ShapeEnum.hxx>
//#include <TopoDS_Wire.hxx>
//#include <TopoDS.hxx>
//#include <TopTools_HSequenceOfShape.hxx>
//#include <TCollection_AsciiString.hxx>
//#include <TColgp_Array1OfPnt.hxx>
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
    Impl() = default;

    std::vector<std::unique_ptr<McCAD::Decomposition::Mesh>> meshList;
    
    void initiate();
    Standard_Boolean generateMesh(const Standard_Real& meshDeflection);

  private:

  };
}

#endif //BOUNDSURFACE_IMPL_HPP
