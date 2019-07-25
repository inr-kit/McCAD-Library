// McCAD
#include "decomposeSolid_impl.hpp"

void
McCAD::Decomposition::DecomposeSolid::Impl::initiate(const TopoDS_Solid& aSolid){
  solid = aSolid;
  meshDeflection = preproc.accessImpl()->calcMeshDeflection(solid);
  //std::cout << "deflection: " << meshDeflection << std::endl;
  // Calculate Length of bounding box.
  Bnd_Box boundingBox;
  BRepBndLib::Add(solid, boundingBox);
  boundingBox.SetGap(0.0);
  boxSquareLength = sqrt(boundingBox.SquareExtent());
  //std::cout << "boxSquareLength: " << boxSquareLength << std::endl;
  perform();
}

void
McCAD::Decomposition::DecomposeSolid::Impl::perform(){
  // The function will be called recursively on a solid and a condition has to be set for termination.
  // Increment the level by 1.
  ++recurrenceDepth;
  std::cout << "     - Recurrence Depth: " << recurrenceDepth << std::endl;
  // Update edges convexity.
  updateEdgesConvexity();
  // Generate the boundary surface list of the solid.
  generateSurfacesList();
  // Judge which surfaces are decompose surfaces from the generated list.
  judgeDecomposeSurfaces();
}

void
McCAD::Decomposition::DecomposeSolid::Impl::updateEdgesConvexity(const Standard_Real& angleTolerance){
  TopTools_IndexedDataMapOfShapeListOfShape mapEdgeFace;
  TopExp::MapShapesAndAncestors(solid, TopAbs_EDGE, TopAbs_FACE, mapEdgeFace);

  Standard_Integer numberEdges = mapEdgeFace.Extent();
  TopTools_ListOfShape facesList;

  for (Standard_Integer edgeNumber = 1; edgeNumber <= numberEdges; ++edgeNumber)
    {
      TopoDS_Edge edge = TopoDS::Edge(mapEdgeFace.FindKey(edgeNumber));
      BRepAdaptor_Curve curveAdaptor;
      curveAdaptor.Initialize(edge);
      facesList = mapEdgeFace.FindFromKey(edge);
      if(facesList.Extent() != 2)
	{
	  continue;
        }
      TopTools_ListIteratorOfListOfShape iterFace(facesList);
      TopoDS_Face firstFace = TopoDS::Face(iterFace.Value());
      iterFace.Next();
      TopoDS_Face secondFace = TopoDS::Face(iterFace.Value());

      Standard_Real start, end;
      Handle_Geom_Curve curve = BRep_Tool::Curve(edge, start, end);
      gp_Pnt startPoint;
      gp_Vec vector;
      curve->D0(start, startPoint);
      curve->D1(start, startPoint, vector);
      gp_Dir direction(vector);

      // Get the normals of each surface
      gp_Dir firstNormal = preproc.accessImpl()->normalOnFace(firstFace, startPoint);
      gp_Dir secondNormal = preproc.accessImpl()->normalOnFace(secondFace, startPoint);
      Standard_Real angle = firstNormal.AngleWithRef(secondNormal, direction);

      if(std::abs(angle) < angleTolerance)
	{
	  angle = Standard_Real(0);
        }
      // The edge is concave.
      if( angle < Standard_Real(0) && edge.Orientation() == TopAbs_REVERSED)
	{
	  edge.Convex(1);
        }
      else if(angle > Standard_Real(0) && edge.Orientation() == TopAbs_FORWARD)
        {
	  edge.Convex(1);
        }
      std::cout << "angle: " << angle << ", convexity " << edge.Convex() << std::endl;
    }
}

void
McCAD::Decomposition::DecomposeSolid::Impl::generateSurfacesList(){
  // Generate a list of faces of the solid.
  TopoDS_Face face;
  Standard_Integer faceNumber = 0;
  std::vector<std::unique_ptr<McCAD::Decomposition::BoundSurface>> planesList;
  TopExp_Explorer explorer(solid, TopAbs_FACE);
  for (; explorer.More(); explorer.Next())
    {
      face = TopoDS::Face(explorer.Current());
      BRepTools::Update(face);
      Standard_Boolean rejectCondition = preproc.accessImpl()->checkFace(face);
      if (!rejectCondition)
	{
	  ++faceNumber;
	  //std::cout << "faceNumber: " << faceNumber << std::endl;
	  preproc.accessImpl()->fixFace(face);
	  //std::cout << "face fixed: " << std::endl;
	  std::unique_ptr<McCAD::Decomposition::BoundSurface> boundSurface = std::move(generateSurface(face));
	  boundSurface->accessSImpl()->initiate(face);
	  boundSurface->accessSImpl()->surfaceNumber = faceNumber;
	  if (boundSurface->accessBSImpl()->generateMesh(meshDeflection))
	    {
	      generateEdges(boundSurface);
	      std::cout << "length of egdes list: " << boundSurface->accessBSImpl()->edgesList.size() << std::endl;
	      if(boundSurface->getSurfaceType() == "Plane")
		{
		  planesList.push_back(std::move(boundSurface));
		}
	      // The other two types in McCADDecompSolid are to be added later.
	    }
	}
      else continue;
    }
  std::cout << "     - There are " << planesList.size() << " planes in the solid" << std::endl;
  mergeSurfaces(planesList);
}

std::unique_ptr<McCAD::Decomposition::BoundSurface>
McCAD::Decomposition::DecomposeSolid::Impl::generateSurface(const TopoDS_Face& face, Standard_Integer mode){
  if (mode == Standard_Integer(0))
    {
      //std::cout << "mode 0 " << std::endl;
      BRepAdaptor_Surface surface(face, Standard_True);
      //std::cout << "BRepAdaptor_Surface" << std::endl;
      GeomAdaptor_Surface AdaptorSurface = surface.Surface();
      //std::cout << "GeomAdaptor_Surface" << std::endl;
      if (AdaptorSurface.GetType() == GeomAbs_Plane)
	{
	  std::cout << preproc.accessImpl()->getSurfTypeName(AdaptorSurface.GetType()) << std::endl;
	  std::unique_ptr<McCAD::Decomposition::BoundSurfacePlane> boundSurfacePlane = std::make_unique<McCAD::Decomposition::BoundSurfacePlane>();
	  boundSurfacePlane->setSurfaceType(boundSurfacePlane->accessBSPImpl()->surfaceType);
	  boundSurfacePlane->accessBSPImpl()->generateExtPlane(boxSquareLength);
	  //assert(boundSurfacePlane);
	  //std::cout << "return poly" << std::endl;
	  return boundSurfacePlane;
	}
      else if (AdaptorSurface.GetType() == GeomAbs_Cylinder)
	{
	  std::cout << preproc.accessImpl()->getSurfTypeName(AdaptorSurface.GetType()) << std::endl;
	  std::unique_ptr<McCAD::Decomposition::BoundSurface> boundSurfaceCylinder = std::make_unique<McCAD::Decomposition::BoundSurface>();
	  return boundSurfaceCylinder;
	}
      else if (AdaptorSurface.GetType() == GeomAbs_Cone)
	{
	  std::cout << preproc.accessImpl()->getSurfTypeName(AdaptorSurface.GetType()) << std::endl;
	  std::unique_ptr<McCAD::Decomposition::BoundSurface> boundSurfaceCone = std::make_unique<McCAD::Decomposition::BoundSurface>();
	  return boundSurfaceCone;
	}
      else
	{
	  std::cout << preproc.accessImpl()->getSurfTypeName(AdaptorSurface.GetType()) << std::endl;
	  std::unique_ptr<McCAD::Decomposition::BoundSurface> boundSurface = std::make_unique<McCAD::Decomposition::BoundSurface>();
          return boundSurface;
	}
    }
  //std::cout << "<<<<<<<<<<<<<< mode 1" << std::endl;
  return nullptr;
}

void
McCAD::Decomposition::DecomposeSolid::Impl::generateEdges(std::unique_ptr<McCAD::Decomposition::BoundSurface>& boundSurface, Standard_Real uvTolerance){
  TopoDS_Face face = boundSurface->accessSImpl()->face;
  TopExp_Explorer explorer(face, TopAbs_EDGE);
  for(; explorer.More(); explorer.Next())
    {
      TopoDS_Edge tempEdge = TopoDS::Edge(explorer.Current());
      std::unique_ptr<McCAD::Decomposition::Edge> edge = std::make_unique<McCAD::Decomposition::Edge>();
      edge->accessEImpl()->initiate(tempEdge);
      // Get type of Edge.
      BRepAdaptor_Curve curveAdaptor;
      curveAdaptor.Initialize(tempEdge);
      edge->setEdgeType(preproc.accessImpl()->getCurveTypeName(curveAdaptor.GetType()));
      edge->accessEImpl()->convexity = tempEdge.Convex();

      // Add flag if the edge can be used for assisting splitting surface.
      if (boundSurface->getSurfaceType() == "Cylinder" && edge->getEdgeType() == "Line")
	{
	  std::vector<Standard_Real> edgeUV(4), surfaceUV(4);
          BRepTools::UVBounds(face, tempEdge, edgeUV[0], edgeUV[1], edgeUV[2], edgeUV[3]);
          BRepTools::UVBounds(face, surfaceUV[0], surfaceUV[1], surfaceUV[2], surfaceUV[3]);
          if (std::abs(edgeUV[0] - surfaceUV[0]) < uvTolerance || std::abs(edgeUV[1] - surfaceUV[1]) < uvTolerance)
	    {
	      edge->accessEImpl()->useForSplitSurface = true;
	    }
	}
      boundSurface->accessBSImpl()->edgesList.push_back(std::move(edge));
    }
}

void
McCAD::Decomposition::DecomposeSolid::Impl::mergeSurfaces(std::vector<std::unique_ptr<McCAD::Decomposition::BoundSurface>>& surfacesList){
  if (surfacesList.size() < 2)
    {
      return;
    }
  for (Standard_Integer i = 0; i <= surfacesList.size() - 2; ++i)
    {
      for (Standard_Integer j = i+1; j <= surfacesList.size() - 1; ++j)
	{
	  if (*(surfacesList[i]) == *(surfacesList[j]))
	    {
	      surfacesList[j]->accessSImpl()->surfaceNumber = surfacesList[i]->accessSImpl()->surfaceNumber;
	      if (*(surfacesList[i]) << *(surfacesList[j]))
		{
		  //surfacesList[i]->accessBSImpl()->fuseSurfaces();

		  // Erase pointer surfacesList[j] from surfacesList.
		  surfacesList.erase(surfacesList.begin() + j);
		  --j;
		  if (surfacesList.size() < 2)
		    {
		      return;
		    }
		}
	    }
	}
    }
}

void
McCAD::Decomposition::DecomposeSolid::Impl::judgeDecomposeSurfaces(){
}
