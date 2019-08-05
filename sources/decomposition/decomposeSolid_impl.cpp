// McCAD
#include "decomposeSolid_impl.hpp"

McCAD::Decomposition::DecomposeSolid::Impl::Impl() : splitSolidList{std::make_unique<TopTools_HSequenceOfShape>()}{
}

McCAD::Decomposition::DecomposeSolid::Impl::~Impl(){
}

Standard_Boolean
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
  return perform();
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::perform(){
  // The function will be called recursively on a solid and a condition has to be set for termination.
  // Increment the level by 1.
  ++recurrenceDepth;
  std::cout << "     - Recurrence Depth: " << recurrenceDepth << std::endl;
  // Update edges convexity.
  updateEdgesConvexity();
  //throw std::runtime_error("saved concave edges");
  // Generate the boundary surface list of the solid.
  generateSurfacesList();
  STEPControl_Writer wrt;
  for (Standard_Integer DD = 0; DD <= facesList.size() - 1; ++DD)
    {
      wrt.Transfer(facesList[DD]->accessSImpl()->face, STEPControl_AsIs);
    }
  std::ostringstream oss;
  oss << "./faceslist" << ".stp";
  std::string listname = oss.str();
  wrt.Write(listname.c_str());
  //throw std::runtime_error("saved facesList");

  // Judge which surfaces are decompose surfaces from the generated list.
  judgeDecomposeSurfaces();
  //std::cout << "length of faceslist: " << facesList.size() << std::endl;
  //std::cout << "length of splitfaceslist: " << splitFacesList.size() << std::endl;
  //std::cout << "splitSurface: " << splitSurface  << std::endl;

  if (!splitSurfaces.accessSSImpl()->throughNoBoundarySurfaces(splitFacesList))
    {
      std::cout << "throughNoBoundarySurfaces false" << std::endl;
      judgeThroughConcaveEdges(facesList);
      if (!splitSurfaces.accessSSImpl()->planeSplitOnlyPlane(splitFacesList))
	{
	  //generateAssistingSurfaces();
	  //judgeAssistingDecomposeSurfaces();
	  //judgeThroughConcaveEdges(assistingFacesList);
	}
    }
  //Standard_Integer selectedSurface = 0;
  
  if (recurrenceDepth >= 5)
    {
      return Standard_False;
    }
  
  if (splitSurface)
    {
      std::cout << "Solid has a split surface" << std::endl;
      if (!selectSplitSurface())
	{
	  return Standard_False;
	}
      // Split the solid with the selected surface.
      std::cout << "selected surface concave edges: " << selectedSplitFacesList[0]->accessSImpl()->throughConcaveEdges << std::endl;
      STEPControl_Writer wrt;
      wrt.Transfer(selectedSplitFacesList[0]->accessSImpl()->face, STEPControl_AsIs);
      std::ostringstream oss;
      oss << "./list_" << recurrenceDepth << ".stp";
      std::string surfname = oss.str();
      wrt.Write(surfname.c_str());
      if (!splitSolid.accessSSImpl()->initiate(solid, selectedSplitFacesList[0], splitSolidList))
	{
	  return Standard_False;
	}

      // Loop over the resulting subsolids and split each one of them recursively.
      std::cout << "splitting subsolids" << std::endl;
      for (Standard_Integer i = 1; i <= splitSolidList->Length(); ++i)
	{
	  TopoDS_Solid subSolid = TopoDS::Solid(splitSolidList->Value(i));   
	  std::unique_ptr<McCAD::Decomposition::DecomposeSolid> decomposedSolid = std::make_unique<McCAD::Decomposition::DecomposeSolid>();
	  decomposedSolid->accessDSImpl()->recurrenceDepth = recurrenceDepth;
	  // mesh deflection is calculated inside initiate for every solid!.
          if (decomposedSolid->accessDSImpl()->initiate(subSolid))
            {
	      splitSolidList->Remove(i);
	      for (Standard_Integer j = 1; j <= decomposedSolid->accessDSImpl()->splitSolidList->Length(); ++j)
		{
		  splitSolidList->Append(decomposedSolid->accessDSImpl()->splitSolidList->Value(j));
		}
	      //return Standard_True;
	    }
	  else
	    {
	      return Standard_False;
	    }
	}
      return Standard_True;
    }
  else
    {
      std::cout	<< "Solid has no split surfaces" << std::endl;
      splitSolidList->Append(solid);
    }
  return Standard_True;
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
      // The edge is convex.
      if( angle < Standard_Real(0) && edge.Orientation() == TopAbs_REVERSED)
	{
	  edge.Convex(1);
        }
      else if(angle > Standard_Real(0) && edge.Orientation() == TopAbs_FORWARD)
        {
	  edge.Convex(1);
        }
      else
	{
	  // edge is concave
	  edge.Convex(0);
	}
      std::cout << "angle: " << angle << ", convexity " << edge.Convex() << std::endl;
      if (edge.Convex() == 0)
	{
	  STEPControl_Writer wrt;
          wrt.Transfer(firstFace, STEPControl_AsIs);
	  wrt.Transfer(secondFace, STEPControl_AsIs);
          std::ostringstream oss;
          oss << "./concave_edge_" << edgeNumber << "_" << recurrenceDepth << ".stp";
          std::string surfname = oss.str();
          wrt.Write(surfname.c_str());
	  //throw std::runtime_error("saved concave edge");
	}
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
	  //boundSurface->accessSImpl()->initiate(face);
	  boundSurface->accessSImpl()->surfaceNumber = faceNumber;
	  if (boundSurface->accessBSImpl()->generateMesh(meshDeflection))
	    {
	      generateEdges(boundSurface);
	      //std::cout << "length of egdes list: " << boundSurface->accessBSImpl()->edgesList.size() << std::endl;
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
  //mergeSurfaces(planesList);
  //std::cout << "merged planes list: " << planesList.size() << std::endl;
  for (Standard_Integer i = 0; i <= planesList.size() - 1; ++i)
    {
      facesList.push_back(std::move(planesList[i]));
    }
  std::cout << "merged faces list: " << facesList.size() << std::endl;
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
	  //std::cout << preproc.accessImpl()->getSurfTypeName(AdaptorSurface.GetType()) << std::endl;
	  std::unique_ptr<McCAD::Decomposition::BoundSurfacePlane> boundSurfacePlane = std::make_unique<McCAD::Decomposition::BoundSurfacePlane>();
	  boundSurfacePlane->setSurfaceType(boundSurfacePlane->accessBSPImpl()->surfaceType);
	  boundSurfacePlane->accessSImpl()->initiate(face);
	  boundSurfacePlane->accessBSPImpl()->generateExtendedPlane(boxSquareLength);
	  //assert(boundSurfacePlane);
	  return boundSurfacePlane;
	}
      else if (AdaptorSurface.GetType() == GeomAbs_Cylinder)
	{
	  //std::cout << preproc.accessImpl()->getSurfTypeName(AdaptorSurface.GetType()) << std::endl;
	  std::unique_ptr<McCAD::Decomposition::BoundSurface> boundSurfaceCylinder = std::make_unique<McCAD::Decomposition::BoundSurface>();
	  boundSurfaceCylinder->accessSImpl()->initiate(face);
	  return boundSurfaceCylinder;
	}
      else if (AdaptorSurface.GetType() == GeomAbs_Cone)
	{
	  //std::cout << preproc.accessImpl()->getSurfTypeName(AdaptorSurface.GetType()) << std::endl;
	  std::unique_ptr<McCAD::Decomposition::BoundSurface> boundSurfaceCone = std::make_unique<McCAD::Decomposition::BoundSurface>();
	  boundSurfaceCone->accessSImpl()->initiate(face);
	  return boundSurfaceCone;
	}
      else
	{
	  //std::cout << preproc.accessImpl()->getSurfTypeName(AdaptorSurface.GetType()) << std::endl;
	  std::unique_ptr<McCAD::Decomposition::BoundSurface> boundSurface = std::make_unique<McCAD::Decomposition::BoundSurface>();
	  boundSurface->accessSImpl()->initiate(face);
          return boundSurface;
	}
    }
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
	  //std::cout << i << " , " << j << " , " << surfacesList.size() << std::endl;
	  if (*(surfacesList[i]) == *(surfacesList[j]))
	    {
	      //std::cout << "equal" << std::endl;
	      surfacesList[j]->accessSImpl()->surfaceNumber = surfacesList[i]->accessSImpl()->surfaceNumber;
	      // Test if the two surfaces can be fused.
	      if (*(surfacesList[i]) << *(surfacesList[j]))
		{
		  if (surfacesList[i]->getSurfaceType() == "Plane")
		    {
		      std::cout << "equal, fuse plane" << std::endl;
		      
		      TopoDS_Face newFace = preproc.accessImpl()->fusePlanes(surfacesList[i]->accessSImpl()->face, surfacesList[j]->accessSImpl()->face);
		      std::unique_ptr<McCAD::Decomposition::BoundSurface> newboundSurface = std::move(generateSurface(newFace));
		      newboundSurface->accessSImpl()->initiate(newFace);
		      newboundSurface->accessSImpl()->surfaceNumber = surfacesList[i]->accessSImpl()->surfaceNumber;
		      if (newboundSurface->accessBSImpl()->generateMesh(meshDeflection))
			{
			  //std::cout << "fuse planes, generate edges" << std::endl;
			  generateEdges(newboundSurface);
			}

		      // Erase pointer surfacesList[j] & [i] from surfacesList.
		      //std::cout << "equal, erase two: " << i << " , " << j << " , " << surfacesList.size() << std::endl;
		      surfacesList.erase(surfacesList.begin() + j);
                      --j;
                      surfacesList.erase(surfacesList.begin() + i);
                      --i;

		      surfacesList.push_back(std::move(newboundSurface));
		      break;
		    }
		}
	      else
		{
		  std::cout << "equal, erase one" << std::endl;
		  // Erase pointer surfacesList[j] from surfacesList.
		  surfacesList.erase(surfacesList.begin() + j);
		  --j;
		}
	      if (surfacesList.size() < 2)
		{
		  return;
		}
	    }
	}
    }
}

void
McCAD::Decomposition::DecomposeSolid::Impl::judgeDecomposeSurfaces(){
  // Judge whether boundary surfaces of the solid can be used for decomposition.
  //std::cout << "judgeDecomposeSurfaces" << std::endl;
  if (facesList.size() < 2)
    {
      return;
    }
  for (Standard_Integer i = 0; i <= facesList.size() - 2; ++i)
    {
      //std::cout << "judge: " << i << " , " << std::endl;
      Standard_Integer positiveFaces = 0;
      Standard_Integer negativeFaces = 0;
      Standard_Integer numberCollidingSurfaces = 0;
      Standard_Integer numberCollidingCurvedSurfaces = 0;
      for (Standard_Integer j = i+1; j <= facesList.size() - 1; ++j)
	{
	  //std::cout << "judge: " << j << " , " << std::endl;
	  if (facesList[i]->accessSImpl()->surfaceNumber != facesList[j]->accessSImpl()->surfaceNumber)
	    {
	      Standard_Integer side = 0;
	      if (facesList[i]->accessBSImpl()->faceCollision(*(facesList[j]), side))
		{
		  ++numberCollidingSurfaces;
		  //std::cout << "facecollision True" << std::endl;
		  facesList[i]->accessSImpl()->splitSurface = Standard_True;
		  if (facesList[j]->getSurfaceType() != "Plane")
		    {
		      ++numberCollidingCurvedSurfaces;
		    }
		}
	      else
		{
		  //std::cout << "facecollision False" << std::endl;
		  //std::cout << side << std::endl;
		  if (side == 1)
		    {
		      ++positiveFaces;
		    }
		  else if (side == -1)
		    {
		      ++negativeFaces;
		    }
		}
	    }
	}
      if (positiveFaces > 0 && negativeFaces > 0)
	{
	  //std::cout << "splitsutface True, pos & neg" << std::endl;
	  facesList[i]->accessSImpl()->splitSurface = Standard_True;
	}
      if (facesList[i]->accessSImpl()->splitSurface)
	{
	  //std::cout << "set collidingsurfaces" << std::endl;
	  facesList[i]->accessSImpl()->numberCollidingSurfaces = numberCollidingSurfaces;
	  facesList[i]->accessSImpl()->numberCollidingCurvedSurfaces = numberCollidingCurvedSurfaces;
	  //std::cout << "adding to split surfaces list" << std::endl;
	  splitFacesList.push_back(facesList[i]);
	  splitSurface = Standard_True;
	}
    }
}

void
McCAD::Decomposition::DecomposeSolid::Impl::judgeThroughConcaveEdges(std::vector<std::shared_ptr<McCAD::Decomposition::BoundSurface>>& facesList){
  // Judge how many concave edges each boundary face of solid goes through.
  for (Standard_Integer i = 0; i <= facesList.size() - 1; ++i)
    {
      Standard_Integer throughConcaveEdges = 0;
      for (Standard_Integer j = 0; j <= facesList.size() - 1; ++j)
	{
	  if (i != j && facesList[i]->accessSImpl()->surfaceNumber != facesList[j]->accessSImpl()->surfaceNumber)
	    {
	      for (Standard_Integer k = 0; k <= facesList[j]->accessBSImpl()->edgesList.size() - 1; ++k)
		{
		  if (facesList[j]->accessBSImpl()->edgesList[k]->accessEImpl()->convexity == 0 && facesList[i]->accessBSImpl()->edgeOnSurface(*(facesList[j]->accessBSImpl()->edgesList[k])))
		    {
		      ++throughConcaveEdges;
		    }
		}
	    }
	}
      facesList[i]->accessSImpl()->throughConcaveEdges = throughConcaveEdges;
      std::cout << "throughConcaveEdges: " << throughConcaveEdges << std::endl;
    }    
}

void
McCAD::Decomposition::DecomposeSolid::Impl::generateAssistingSurfaces(){
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::selectSplitSurface(){
  // mergeSplitSurfaces(splitFacesList) // see no need for it as the formed lists; planesList, etc. are already merged. splitFacesList is a subset of facesList.
  splitSurfaces.accessSSImpl()->generateSplitFacesList(splitFacesList, selectedSplitFacesList);
  if (!selectedSplitFacesList.empty())
    {
      return Standard_True;
    }
  return Standard_False;
}
