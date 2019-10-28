// McCAD
#include "boundSurface_impl.hpp"
#include "faceeCollision.hpp"

McCAD::Geometry::BoundSurface::Impl::Impl(BoundSurface* backReference)
  : boundSurface{backReference}{
}

McCAD::Geometry::BoundSurface::Impl::~Impl(){
}

Standard_Boolean
McCAD::Geometry::BoundSurface::Impl::isEqual(const McCAD::Geometry::BoundSurface& that){
    Standard_Boolean equalityCondition
            = Tools::PlaneComparator{}(boundSurface->accessSImpl()->face,
                                       that.accessSImpl()->face);
  return equalityCondition;
}

Standard_Boolean
McCAD::Geometry::BoundSurface::Impl::canFuse(const McCAD::Geometry::BoundSurface& that){
    Standard_Boolean equalityCondition = Tools::PlaneComparator{}(
                boundSurface->accessSImpl()->face, that.accessSImpl()->face);
  if (!equalityCondition){
      return Standard_False;
  }
  // Check common edges of the two faces.
  for (Standard_Integer i = 0; i <= edgesList.size() - 2; ++i){
      for (Standard_Integer j = i+1; j <= that.accessBSImpl()->edgesList.size() - 1;
           ++j){
          if (*edgesList[i] == *that.accessBSImpl()->edgesList[j]){
              return Standard_True;
          }
      }
  }
  return Standard_False;
}

Standard_Boolean
McCAD::Geometry::BoundSurface::Impl::faceCollision(const BoundSurface& aFace,
                                                   Standard_Integer& aSide){
    Standard_Boolean collision = Standard_False;
    Standard_Integer positiveTriangles = 0;
    Standard_Integer negativeTriangles = 0;
    auto& meshTriangleList = aFace.accessBSImpl()->meshTrianglesList;
    for (Standard_Integer i = 0; i <= meshTriangleList.size() - 1; ++i){
        Standard_Integer side = 0;
        if (triangleCollision(*(meshTriangleList[i]), side)){
            collision = Standard_True;
            break;
        } else{
            if (side == 1){
                ++positiveTriangles;
            } else if (side == -1){
                ++negativeTriangles;
            }
        }
        if (positiveTriangles > 0 && negativeTriangles > 0){
            collision = Standard_True;
            break;
        }
    }
    // Update side.
    if (positiveTriangles > 0 && negativeTriangles == 0){
        aSide = 1;
    } else if (positiveTriangles == 0 && negativeTriangles > 0){
        aSide = -1;
    }
    return collision;
}

Standard_Boolean
McCAD::Geometry::BoundSurface::Impl::generateMesh(const Standard_Real& meshDeflection){
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
	  // Get mesh triangles.
	  Standard_Integer numberTriangles = mesh->NbTriangles();
	  const Poly_Array1OfTriangle& Triangles = mesh->Triangles();
	  std::array<Standard_Integer, 3> triangleNodes;
	  for (const auto& Triangle : Triangles)
	    {
	      Triangle.Get(triangleNodes[0], triangleNodes[1], triangleNodes[2]);
	      //std::cout << triangleNodes[0] << std::endl;
	      //std::cout	<< triangleNodes[1] << std::endl;
	      //std::cout	<< triangleNodes[2] << std::endl;
	      //std::cout << "=====" << std::endl;
	      std::array<gp_Pnt, 3> points = {
		meshNodes(triangleNodes[0]).Transformed(Transformation),
		meshNodes(triangleNodes[1]).Transformed(Transformation),
		meshNodes(triangleNodes[2]).Transformed(Transformation)};
	      
	      // Generate new face with the retrieved triangle points.
	      TopoDS_Wire wire = BRepBuilderAPI_MakePolygon(points[0], points[1],
							    points[2], Standard_True);
	      TopoDS_Face triangleFace = BRepBuilderAPI_MakeFace(wire, Standard_True);
	      std::unique_ptr<MeshTriangle> meshTriangle = std::make_unique<MeshTriangle>();
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
      //std::cout << "cannot mesh surface" << std::endl;
      return Standard_False;
    }
}

void
McCAD::Geometry::BoundSurface::Impl::generateEdges(Standard_Real uvTolerance){
    TopoDS_Face face = boundSurface->accessSImpl()->face;
    for (const auto& tempEdge : ShapeView<TopAbs_EDGE>{face}){
        std::unique_ptr<Edge> edge = std::make_unique<Edge>();
        edge->accessEImpl()->initiate(tempEdge);
        // Get type of Edge.
        BRepAdaptor_Curve curveAdaptor(tempEdge);
        edge->setEdgeType(Tools::toTypeName(curveAdaptor.GetType()));
        edge->accessEImpl()->convexity = tempEdge.Convex();
        if (tempEdge.Convex() == Standard_Integer(0)){
            boundSurface->accessSImpl()->throughConcaveEdges += 1;
        }
        // Add flag if the edge can be used for assisting splitting surface.
        if (boundSurface->getSurfaceType() == Tools::toTypeName(GeomAbs_Cylinder) &&
                edge->getEdgeType() == Tools::toTypeName(GeomAbs_Line)){
            std::array<Standard_Real, 4> edgeUV, surfaceUV;
            BRepTools::UVBounds(face, tempEdge, edgeUV[0], edgeUV[1], edgeUV[2],
                    edgeUV[3]);
            BRepTools::UVBounds(face, surfaceUV[0], surfaceUV[1], surfaceUV[2],
                    surfaceUV[3]);
            if (std::abs(edgeUV[0] - surfaceUV[0]) < uvTolerance ||
                    std::abs(edgeUV[1] - surfaceUV[1]) < uvTolerance){
                edge->accessEImpl()->useForSplitSurface = Standard_True;
            }
        }
        edgesList.push_back(std::move(edge));
    }
}
/*
// This function is used as virtual one in BndSurfPlane. Should be modified later.
Standard_Boolean
McCAD::Geometry::BoundSurface::Impl::triangleCollision(const MeshTriangle& aTriangle,
						       Standard_Integer& aSide,
						       Standard_Real tolerance,
						       Standard_Real tolerance2){
  //std::cout << "triangleCollision" << std::endl;
  Standard_Boolean collision = Standard_False;
  Standard_Integer positivePoints = 0;
  Standard_Integer negativePoints = 0;

  for (Standard_Integer i = 0; i <= aTriangle.accessMTImpl()->points.size() - 1; ++i)
    {
      if (pointOnSurface(aTriangle.accessMTImpl()->points[i], tolerance))
	{
	  continue;
	}

      // Evaluate. Should be a seperate function.
      Standard_Real evaluate;
      BRepAdaptor_Surface surfaceAdaptor(boundSurface->accessSImpl()->face, Standard_True);
      GeomAdaptor_Surface surfaceGeomAdaptor= surfaceAdaptor.Surface();
      gp_Pln Plane = surfaceGeomAdaptor.Plane();
      std::array<Standard_Real, 4> parameters;
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
  //std::cout << "triangleCollision return" << std::endl;
  return collision;
}

Standard_Boolean
McCAD::Geometry::BoundSurface::Impl::pointOnSurface(const gp_Pnt& aPoint, const Standard_Real& distanceTolerance){
  //std::cout << "pointOnSurface" << std::endl;
  BRepAdaptor_Surface surfaceAdaptor(boundSurface->accessSImpl()->extendedFace,
				     Standard_True);
  Standard_Real uvTolerance = surfaceAdaptor.Tolerance();
  std::array<Standard_Real, 4> uvParameters;
  uvParameters[0] = surfaceAdaptor.FirstUParameter();
  uvParameters[1] = surfaceAdaptor.LastUParameter();
  uvParameters[2] = surfaceAdaptor.FirstVParameter();
  uvParameters[3] = surfaceAdaptor.LastVParameter();

  Extrema_ExtPS extremumDistances(aPoint, surfaceAdaptor, uvParameters[0],
				  uvParameters[1], uvParameters[2], uvParameters[3],
				  uvTolerance, uvTolerance);
  if (extremumDistances.IsDone() && extremumDistances.NbExt() != 0)
    {
      gp_Pnt point = extremumDistances.Point(1).Value();
      Standard_Real distance = std::sqrt(std::pow(aPoint.X() - point.X(), 2) +
					 std::pow(aPoint.Y() - point.Y(), 2) +
					 std::pow(aPoint.Z() - point.Z(), 2));
      if (distance < distanceTolerance)
	{
	  return Standard_True;
	}
    }
  //std::cout << "pointOnSurface return"	<< std::endl;
  return Standard_False;
}*/

Standard_Boolean
McCAD::Geometry::BoundSurface::Impl::edgeOnSurface(const Edge& aEdge,
						   Standard_Real tolerance){
  gp_Pnt startPoint = aEdge.accessEImpl()->startPoint;
  gp_Pnt endPoint   = aEdge.accessEImpl()->endPoint;
  //std::cout << startPoint.X() << "," << startPoint.Y() << "," << startPoint.Z() << "," << std::endl;
  //std::cout << endPoint.X() << "," << endPoint.Y() << "," << endPoint.Z() << "," << std::endl;
  tolerance = BRep_Tool::Tolerance(boundSurface->accessSImpl()->extendedFace);
  if (pointOnSurface(startPoint, tolerance) && pointOnSurface(endPoint, tolerance))
    {
      //std::cout << "points on surface" << std::endl;
      if (aEdge.accessEImpl()->edgeType == "Line")
	{
	  //std::cout	<< "line" << std::endl;
	  return Standard_True;
	}
      else
	{
	  gp_Pnt middlePoint = aEdge.accessEImpl()->middlePoint;
          gp_Pnt extraPoint = aEdge.accessEImpl()->extraPoint;
          if (pointOnSurface(middlePoint, tolerance) && pointOnSurface(extraPoint,
								       tolerance))
	    {
	      //std::cout << "no line" << std::endl;
	      return Standard_True;
	    }
	}
    }
  return Standard_False;
}

void
McCAD::Geometry::BoundSurface::Impl::combineEdges(std::vector<std::unique_ptr<Edge>>& aEdgesList){
  if (edgesList.size() == 0)
    {
      // If the current list is empty, append to it the new one.
      for (Standard_Integer i = 0; i <= aEdgesList.size() - 1; ++i)
	{
	  edgesList.push_back(std::move(aEdgesList[i]));
	}
    }
  else
    {
      // Compare and add only if different.
      for (Standard_Integer i = 0; i <= aEdgesList.size() - 1; ++i)
	{
	  Standard_Integer sameEdge = 0;
	  for (Standard_Integer j = 0; j <= edgesList.size() - 1; ++j)
	    {
	      if (*(edgesList[j]) == *(aEdgesList[i]))
		{
		  edgesList.erase(edgesList.begin() + j);
		  ++sameEdge;
		  --j;
		}
	    }
	  if (sameEdge == 0)
	    {
	      edgesList.push_back(std::move(aEdgesList[i]));
	    }
	}
    }
}
