// McCAD
#include "decomposeSolid_impl.hpp"

McCAD::Decomposition::DecomposeSolid::Impl::Impl(McCAD::Decomposition::DecomposeSolid* decomposeSolid) : decomposeSolid{decomposeSolid}{
}

McCAD::Decomposition::DecomposeSolid::Impl::~Impl(){
}

void
McCAD::Decomposition::DecomposeSolid::Impl::initiate(const TopoDS_Solid& aSolid){
  McCAD::Tools::Preprocessor preproc;
  solid = aSolid;
  decomposeSolid->meshDeflection = preproc.accessImpl()->calcMeshDeflection(solid);
  // Calculate Length of bounding box.
  Bnd_Box boundingBox;
  BRepBndLib::Add(solid, boundingBox);
  boundingBox.SetGap(0.0);
  decomposeSolid->boxSquareLength = sqrt(boundingBox.SquareExtent());
  
  perform();
}

void
McCAD::Decomposition::DecomposeSolid::Impl::perform(){
  // The function will be called recursively on a solid and a condition has to be set for termination.
  // Increment the level by 1.
  ++(decomposeSolid->recurrenceDepth);
  std::cout << "     - Recurrence Depth: " << decomposeSolid->recurrenceDepth << std::endl;
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
  Standard_Integer faceNumber = 0;
  std::vector<McCAD::Decomposition::BoundSurface*> planesList;
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
	  McCAD::Decomposition::BoundSurface* boundSurface = generateSurface(face);
	  boundSurface->surfaceNumber = faceNumber;
	  if (boundSurface->accessImpl()->generateMesh(decomposeSolid->meshDeflection))
	    {
	      generateEdges(boundSurface);
	      if(boundSurface->surfaceType == "Plane")
		{
		  planesList.push_back(boundSurface);
		}
	      // The other two types in McCADDecompSolid are to be added later.
	    }
	}
      else continue;
    }
  mergeSurfaces(planesList);
  // The other two types in McCADDecompSolid are to be added later.

  facesList.insert(facesList.end(), planesList.begin(), planesList.end());
}

McCAD::Decomposition::BoundSurface*
McCAD::Decomposition::DecomposeSolid::Impl::generateSurface(const TopoDS_Face& face, Standard_Integer mode){
  if (mode == Standard_Integer(0))
    {
      BRepAdaptor_Surface surface(face, Standard_True);
      GeomAdaptor_Surface AdaptorSurface = surface.Surface();
      if (AdaptorSurface.GetType() == GeomAbs_Plane)
	{
	  McCAD::Decomposition::BoundSurfacePlane boundSurfacePlane;
	  boundSurfacePlane.accessImpl()->initiate(face);
	  boundSurfacePlane.surfaceType = "Plane";
	  boundSurfacePlane.accessImpl()->generateExtPlane(decomposeSolid->boxSquareLength);
	  assert(boundSurfacePlane);
	  return &boundSurfacePlane;
	}
      if (AdaptorSurface.GetType() == GeomAbs_Cylinder)
	{
	}
      if (AdaptorSurface.GetType() == GeomAbs_Cone)
	{
	}
    }
}

void
McCAD::Decomposition::DecomposeSolid::Impl::judgeDecomposeSurfaces(){
}

void
McCAD::Decomposition::DecomposeSolid::Impl::generateEdges(McCAD::Decomposition::BoundSurface* boundSurface){
}

void
McCAD::Decomposition::DecomposeSolid::Impl::mergeSurfaces(std::vector<McCAD::Decomposition::BoundSurface*> planesList){
}
