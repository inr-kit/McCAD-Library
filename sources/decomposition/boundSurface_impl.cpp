// McCAD
#include "boundSurface_impl.hpp"

McCAD::Decomposition::BoundSurface::Impl::Impl(McCAD::Decomposition::BoundSurface* backReference) : boundSurface{backReference}{
}

McCAD::Decomposition::BoundSurface::Impl::~Impl(){
}

Standard_Boolean
McCAD::Decomposition::BoundSurface::Impl::generateMesh(const Standard_Real& meshDeflection){
  // Get surface from base class; Surface.
  TopoDS_Face face = boundSurface->accessSImpl()->face;
  Handle_Poly_Triangulation mesh;

  BRepAdaptor_Surface surface(face, Standard_True);
  gp_Trsf T = surface.Trsf();

  // Generate mesh of the surface.
  try
    {
      TopLoc_Location location;
      BRepMesh::Mesh(face, meshDeflection);
      mesh = BRep_Tool::Triangulation(face, location);
      if (!mesh.IsNull())
	{
	  Standard_Integer numberNodes = mesh->NbNodes();
	  return Standard_True;  
	}
    }
  catch(...)
    {
      std::cout << "cannot mesh surface" << std::endl;
      return Standard_False;
    }

}
