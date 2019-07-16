// McCAD
#include "decomposeSolid_impl.hpp"

void
McCAD::Decomposition::DecomposeSolid::Impl::initiate(const TopoDS_Solid& aSolid){
  solid = aSolid;
  meshDeflection = preproc.accessImpl()->calcMeshDeflection(solid);
  // Calculate Length of bounding box.
  Bnd_Box boundingBox;
  BRepBndLib::Add(solid, boundingBox);
  boundingBox.SetGap(0.0);
  boxSquareLength = sqrt(boundingBox.SquareExtent());
  perform();
}

void
McCAD::Decomposition::DecomposeSolid::Impl::perform(){
  // The function will be called recursively on a solid and a condition has to be set for termination.
  // Increment the level by 1.
  ++recurrenceDepth;
  std::cout << "     - Recurrence Depth: " << recurrenceDepth << std::endl;
  // Generate the boundary surface list of the solid.
  generateSurfacesList();
  // Judge which surfaces are decompose surfaces from the generated list.
  judgeDecomposeSurfaces();
}

void
McCAD::Decomposition::DecomposeSolid::Impl::generateSurfacesList(){
  // Generate a list of faces of the solid.
  TopoDS_Face face;
  Standard_Integer faceNumber = 0;
  std::vector<std::unique_ptr<McCAD::Decomposition::BoundSurface>> planesList;
  //std::vector<TopoDS_Face> cylindersList;
  //std::vector<TopoDS_Face> conesList;
  TopExp_Explorer explorer(solid, TopAbs_FACE);
  for (; explorer.More(); explorer.Next())
    {
      face = TopoDS::Face(explorer.Current());
      BRepTools::Update(face);
      Standard_Boolean rejectCondition = preproc.accessImpl()->checkFace(face);
      if (!rejectCondition)
	{
	  ++faceNumber;
	  preproc.accessImpl()->fixFace(face);
	  std::unique_ptr<McCAD::Decomposition::BoundSurface> boundSurface = std::move(generateSurface(face));
	  boundSurface->accessSImpl()->surfaceNumber = faceNumber;
	  if (boundSurface->accessBSImpl()->generateMesh(meshDeflection))
	    {
	      //generateEdges(boundSurface);
	      if(boundSurface->getSurfaceType() == "Plane")
		{
		  std::cout << "      Plane" << std::endl;
		  //planesList.push_back(boundSurface);
		}
	      // The other two types in McCADDecompSolid are to be added later.
	    }
	}
      else continue;
    }
  //mergeSurfaces(planesList);
  // The other two types in McCADDecompSolid are to be added later.
  //facesList.insert(facesList.end(), planesList.begin(), planesList.end());
}

std::unique_ptr<McCAD::Decomposition::BoundSurface>
McCAD::Decomposition::DecomposeSolid::Impl::generateSurface(const TopoDS_Face& face, Standard_Integer mode){
  if (mode == Standard_Integer(0))
    {
      BRepAdaptor_Surface surface(face, Standard_True);
      GeomAdaptor_Surface AdaptorSurface = surface.Surface();
      if (AdaptorSurface.GetType() == GeomAbs_Plane)
	{
	  std::unique_ptr<McCAD::Decomposition::BoundSurfacePlane> boundSurfacePlane = std::make_unique<McCAD::Decomposition::BoundSurfacePlane>();
	  boundSurfacePlane->setSurfaceType(boundSurfacePlane->accessBSPImpl()->surfaceType);
	  boundSurfacePlane->accessBSPImpl()->initiate(face);
	  boundSurfacePlane->accessBSPImpl()->generateExtPlane(boxSquareLength);
	  //assert(boundSurfacePlane);
	  return boundSurfacePlane;
	}
      else if (AdaptorSurface.GetType() == GeomAbs_Cylinder)
	{
	  return nullptr;
	}
      else if (AdaptorSurface.GetType() == GeomAbs_Cone)
	{
	  return nullptr;
	}
    }
  return nullptr;
}

void
McCAD::Decomposition::DecomposeSolid::Impl::judgeDecomposeSurfaces(){
}

void
McCAD::Decomposition::DecomposeSolid::Impl::generateEdges(){
}

void
McCAD::Decomposition::DecomposeSolid::Impl::mergeSurfaces(){
}
