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
McCAD::Decomposition::BoundSurface::Impl::faceCollision(const McCAD::Decomposition::BoundSurface& aFace, Standard_Integer& aSide)
{
  std::cout << "face collision" << std::endl;
  Standard_Boolean collision = Standard_False;
  Standard_Integer positiveTriangles = 0;
  Standard_Integer negativeTriangles = 0;

  for (Standard_Integer i = 0; i <= aFace.accessBSImpl()->meshTrianglesList.size() - 1; ++i)
    {
      Standard_Integer side = 0;
      if (triangleCollision(*(aFace.accessBSImpl()->meshTrianglesList[i]), side))
	{
	  collision = Standard_True;
	  break;
	}
      else
	{
	  if (side == 1)
	    {
	      ++positiveTriangles;
	    }
	  else if (side == -1)
	    {
	      ++negativeTriangles;
	    }
	}
      if (positiveTriangles > 0 && negativeTriangles > 0)
	{
	  collision = Standard_True;
	  break;
	}
    }
  // Update side.
  if (positiveTriangles > 0 && negativeTriangles == 0)
    {
      aSide = 1;
    }
  else if (positiveTriangles == 0 && negativeTriangles > 0)
    {
      aSide = -1;
    }
  return collision;
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

// This function is used as virtual one in BndSurfPlane. Should be modified later.
Standard_Boolean
McCAD::Decomposition::BoundSurface::Impl::triangleCollision(const McCAD::Decomposition::MeshTriangle& aTriangle, Standard_Integer& aSide, Standard_Real tolerance, Standard_Real tolerance2){
  std::cout << "triangle collision" << std::endl;
  Standard_Boolean collision = Standard_False;
  Standard_Integer positivePoints = 0;
  Standard_Integer negativePoints = 0;

  for (Standard_Integer i = 0; i <= aTriangle.accessMTImpl()->points.size() - 1; ++i)
    {
      if (pointOnSurface(aTriangle.accessMTImpl()->points[i], tolerance))
	{
	  continue;
	}

      // Evaluate. Should be s perate function.
      Standard_Real evaluate;
      BRepAdaptor_Surface surfaceAdaptor(boundSurface->accessSImpl()->face, Standard_True);
      GeomAdaptor_Surface surfaceGeomAdaptor= surfaceAdaptor.Surface();
      gp_Pln Plane = surfaceGeomAdaptor.Plane();
      std::vector<Standard_Real> parameters(4);
      Plane.Coefficients(parameters[0], parameters[1], parameters[2], parameters[3]);
      evaluate = aTriangle.accessMTImpl()->points[i].X()*parameters[0] + \
	aTriangle.accessMTImpl()->points[i].Y()*parameters[1] +		\
	aTriangle.accessMTImpl()->points[i].Z()*parameters[2] + parameters[3];
      if (evaluate > tolerance2)
	{
	  ++positivePoints;
	}
      else if (evaluate < tolerance2)
	{
	  ++negativePoints;
	}
      if (positivePoints > 0 && negativePoints > 0)
	{
	  collision = Standard_True;
	  break;
	} 
    }
  if (positivePoints > 0 && negativePoints == 0)
    {
      aSide = 1;
    }
  else if (positivePoints == 0 && negativePoints > 0)
    {
      aSide = -1;
    }
  return collision;
}

Standard_Boolean
McCAD::Decomposition::BoundSurface::Impl::pointOnSurface(const gp_Pnt& aPoint, const Standard_Real& distanceTolerance){
  std::cout << "point on surface" << std::endl;
  BRepAdaptor_Surface surfaceAdaptor(boundSurface->accessSImpl()->extendedFace, Standard_True);
  Standard_Real uvTolerance = surfaceAdaptor.Tolerance();
  std::vector<Standard_Real> uvParameters(4);
  uvParameters[0] = surfaceAdaptor.FirstUParameter();
  uvParameters[1] = surfaceAdaptor.LastUParameter();
  uvParameters[2] = surfaceAdaptor.FirstVParameter();
  uvParameters[3] = surfaceAdaptor.LastVParameter();

  Extrema_ExtPS extremumDistances(aPoint, surfaceAdaptor, uvParameters[0], uvParameters[1], uvParameters[2], uvParameters[3], uvTolerance, uvTolerance);
  if (extremumDistances.IsDone() && extremumDistances.NbExt() != 0)
    {
      gp_Pnt point = extremumDistances.Point(1).Value();
      Standard_Real distance = std::sqrt(std::pow(aPoint.X() - point.X(), 2) + std::pow(aPoint.Y() - point.Y(), 2) + std::pow(aPoint.Z() - point.Z(), 2));
      if (distance < distanceTolerance)
	{
	  return Standard_True;
	}
    }
  return Standard_False;
}
