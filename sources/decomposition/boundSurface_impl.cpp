// McCAD
#include "boundSurface_impl.hpp"

McCAD::Decomposition::BoundSurface::Impl::Impl(McCAD::Decomposition::BoundSurface* backReference) : boundSurface{backReference}{
}

McCAD::Decomposition::BoundSurface::Impl::~Impl(){
}

Standard_Boolean
McCAD::Decomposition::BoundSurface::Impl::isEqual(const McCAD::Decomposition::BoundSurface& that){
  Standard_Boolean equalityCondition = preproc.accessImpl()->isSamePlane(boundSurface->accessSImpl()->face, that.accessSImpl()->face);
  return equalityCondition;
}

Standard_Boolean
McCAD::Decomposition::BoundSurface::Impl::canFuse(const McCAD::Decomposition::BoundSurface& that){
  Standard_Boolean equalityCondition = preproc.accessImpl()->isSamePlane(boundSurface->accessSImpl()->face, that.accessSImpl()->face);
  if (!equalityCondition)
    {
      return Standard_False;
    }
  // Check common edges of the two faces.
  for (Standard_Integer i = 0; i <= edgesList.size() - 2; ++i)
    {
      for (Standard_Integer j = i+1; j <= that.accessBSImpl()->edgesList.size() - 1; ++j)
	{
	  if (*(edgesList[i]) == *(that.accessBSImpl()->edgesList[j]))
	    {
	      return Standard_True;
	    }
	}
    }
  return Standard_False;
}

Standard_Boolean
McCAD::Decomposition::BoundSurface::Impl::generateMesh(const Standard_Real& meshDeflection){
  // Get surface from base class; Surface.
  TopoDS_Face face = boundSurface->accessSImpl()->face;

  // Generate mesh of the surface.
  try
    {
      TopLoc_Location location;
      Handle_Poly_Triangulation mesh;
      BRepMesh_IncrementalMesh incMesh(face, meshDeflection, Standard_True);
      incMesh.Perform();
      mesh = BRep_Tool::Triangulation(face, location);
      if (!mesh.IsNull())
	{
	  BRepAdaptor_Surface surface(face, Standard_True);
          gp_Trsf Transformation = surface.Trsf();
	  // Get mesh nodes.
	  Standard_Integer numberNodes = mesh->NbNodes();
	  TColgp_Array1OfPnt meshNodes(1, numberNodes);
	  meshNodes = mesh->Nodes();
	  std::cout << "number of nodes: " << numberNodes << std::endl;
	  // Get mesh triangles.
	  Standard_Integer numberTriangles = mesh->NbTriangles();
	  const Poly_Array1OfTriangle& Triangles = mesh->Triangles();
	  std::cout << "number of triangles: " << numberTriangles << std::endl;

	  std::vector<Standard_Integer> triangleNodes(3);
	  for (Standard_Integer i = 1; i <= numberTriangles; ++i)
	    {
	      Poly_Triangle Triangle = Triangles(i);
	      Triangle.Get(triangleNodes[0], triangleNodes[1], triangleNodes[2]);
	      std::cout << triangleNodes[0] << std::endl;
	      std::cout	<< triangleNodes[1] << std::endl;
	      std::cout	<< triangleNodes[2] << std::endl;
	      std::cout << "=====" << std::endl;
	      std::vector<gp_Pnt> points = {
		meshNodes(triangleNodes[0]).Transformed(Transformation),
		meshNodes(triangleNodes[1]).Transformed(Transformation),
		meshNodes(triangleNodes[2]).Transformed(Transformation)};
	      
	      // Generate new face with the retrieved triangle points.
	      TopoDS_Wire wire = BRepBuilderAPI_MakePolygon(points[0], points[1], points[2], Standard_True);
	      TopoDS_Face triangleFace = BRepBuilderAPI_MakeFace(wire, Standard_True);
	      std::unique_ptr<McCAD::Decomposition::MeshTriangle> meshTriangle = std::make_unique<McCAD::Decomposition::MeshTriangle>();
	      meshTriangle->accessMTImpl()->initiate(triangleFace);
	      meshTriangle->accessMTImpl()->points = points; 
	      meshTrianglesList.push_back(std::move(meshTriangle));
	    }      
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
