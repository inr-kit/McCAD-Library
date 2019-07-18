// McCAD
#include "boundSurface_impl.hpp"

McCAD::Decomposition::BoundSurface::Impl::Impl(McCAD::Decomposition::BoundSurface* backReference) : boundSurface{backReference}{
}

McCAD::Decomposition::BoundSurface::Impl::~Impl(){
}

Standard_Boolean
McCAD::Decomposition::BoundSurface::Impl::generateMesh(const Standard_Real& meshDeflection){
  // Get surface from base class; Surface.
  TopLoc_Location location;
  Handle_Poly_Triangulation mesh;
  TopoDS_Face face = boundSurface->accessSImpl()->face;

  // Generate mesh of the surface.
  try
    {
      //BRepMesh::Mesh(face, meshDeflection);
      BRepMesh_IncrementalMesh incMesh(face, meshDeflection, Standard_True);
      incMesh.Perform();
      mesh = BRep_Tool::Triangulation(face, location);
      if (!mesh.IsNull())
	{
	  BRepAdaptor_Surface surface(face, Standard_True);
          gp_Trsf Trnsformation = surface.Trsf();
	  Standard_Integer numberNodes = mesh->NbNodes();
	  TColgp_Array1OfPnt meshPoints(1, numberNodes);
	  meshPoints = mesh->Nodes();
	  Standard_Integer numberTriangles = mesh->NbTriangles();
	  return Standard_True;  
	}
      else
	{
	  return Standard_False;
	}
    }
  catch(...)
    {
      std::cout << "cannot mesh surface" << std::endl;
      return Standard_False;
    }

}
