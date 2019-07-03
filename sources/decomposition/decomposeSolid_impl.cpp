// McCAD
#include "decomposeSolid_impl.hpp"
#include "tools_impl.hpp"

void
McCAD::Decomposition::DecomposeSolid::Impl::initiate(const TopoDS_Solid& aSolid){
  McCAD::Tools::Preprocessor preproc;
  solid = aSolid;
  meshDeflection = preproc.accessImpl()->calcMeshDeflection(solid);
  // Calculate Length of bounding box
  Bnd_Box boundingBox;
  BRepBndLib::Add(solid, boundingBox);
  boundingBox.SetGap(0.0);
  boxSquareLength = sqrt(boundingBox.SquareExtent());
  
  perform(recurrenceLevel);
}

void
McCAD::Decomposition::DecomposeSolid::Impl::perform(Standard_Integer recurrenceLevel){
  // The function will be called recursively on a solid and a condition has to be set for termination.
  // Increment the level by 1.
  ++recurrenceLevel;
  std::cout << "Level: " << recurrenceLevel << std::endl;
  // Generate the boundary surface list of the solid.
  generateSurfacesList();
  // Judge which surfaces are decompose surfaces from the generated list.
  judgeDecomposeSurfaces();
  
}

void
McCAD::Decomposition::DecomposeSolid::Impl::generateSurfacesList(){
  // Generate a list of faces of the solid.
  McCAD::Tools::Preprocessor preproc;
  TopoDS_Face face;
  Standard_Integer faceNumber;
  std::vector<TopoDS_Face> facesList;
  std::vector<TopoDS_Face> planesList;
  std::vector<TopoDS_Face> cylindersList;
  std::vector<TopoDS_Face> conesList;
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
	  McCAD::Decomposition::BoundSurface* boundSurface = generateSurface(face);
	  boundSurface->accessImpl()->surfaceNumber = faceNumber;
	  if (boundSurface->generateMesh(meshDeflection))
	    {
	      GenEdges(pBndSurf);
	      if(boundSurface->getSurfaceType() == Plane)
		{
		  planesList.push_back(boundSurface);
		}
	      // The other two types in McCADDecompSolid are to be added later.
	    }
	}
      else continue;
    }
  mergeSurfaces(m_PlaneList);
  // The other two types in McCADDecompSolid are to be added later.

  facesList.insert(facesList.end(), planesList.begin(), planesList.end());
}

McCAD::Decomposition::BoundSurface*
McCAD::Decomposition::DecomposeSolid::Impl::generateSurface(const TopoDS_Face& face, Standard_Integer mode){ 
  if (mode == 0)
    {
      BRepAdaptor_Surface surface(face, Standard_True);
      GeomAdaptor_Surface AdaptorSurface = surface.Surface();
      if (surface.GetType() == GeomAbs_Plane)
	{
	  McCAD::Decomposition::BoundSurfacePlane boundSurfacePlane;
	  boundSurfacePlanepreproc.accessImpl()->initiate(face);
	  pBndPlnSurf->GenExtPlane(boxSquareLength);

	  assert(boundSurfacePlane);
	  return boundSurfacePlane;
	}
      if (surface.GetType() == GeomAbs_Cylinder)
	{
	}
      if (surface.GetType() == GeomAbs_Cone)
	{
	}
    }
  
}

void
McCAD::Decomposition::DecomposeSolid::Impl::judgeDecomposeSurfaces(){
}
