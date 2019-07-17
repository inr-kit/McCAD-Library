// McCAD
#include "boundSurface_impl.hpp"

void
McCAD::Decomposition::BoundSurface::Impl::initiate(){
}

Standard_Boolean
McCAD::Decomposition::BoundSurface::Impl::generateMesh(const TopoDS_Face& face, const Standard_Real& meshDeflection){
  Handle_Poly_Triangulation mesh;
  BRepAdaptor_Surface surface(face, Standard_True);
  gp_Trsf T = surface.Trsf();

  // Generate mesh of the surface.
  try
    {
      TopLoc_Location location;
      BRepMesh::Mesh(face, meshDeflection);
      mesh = BRep_Tool::Triangulation(face, location);

      if (mesh.IsNull())
    }
  catch()
    {
      return Standard_False;
    }
  return Standard_True;
}
