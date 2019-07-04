// McCAD
#include "decomposeSolid_impl.hpp"
#include "tools_impl.hpp"
#include "surface_impl.hpp"
#include "boundSurface_impl.hpp"
#include "boundSurfacePlane_impl.hpp"

void
McCAD::Decomposition::DecomposeSolid::Impl::initiate(const TopoDS_Solid& aSolid){
  McCAD::Tools::Preprocessor preproc;
  solid = aSolid;
  meshDeflection = preproc.accessImpl()->calcMeshDeflection(solid);
  // Calculate Length of bounding box.
  Bnd_Box boundingBox;
  BRepBndLib::Add(solid, boundingBox);
  boundingBox.SetGap(0.0);
  boxSquareLength = sqrt(boundingBox.SquareExtent());
  
  perform(recurrenceDepth);
}

void
McCAD::Decomposition::DecomposeSolid::Impl::perform(Standard_Integer recurrenceDepth){
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
	  boundSurface->setSurfaceNumber(faceNumber);
	  if (boundSurface->accessImpl()->generateMesh(meshDeflection))
	    {
	      generateEdges(boundSurface);
	      if(boundSurface->getSurfaceType() == "Plane")
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
  if (mode == 0)
    {
      BRepAdaptor_Surface surface(face, Standard_True);
      GeomAdaptor_Surface AdaptorSurface = surface.Surface();
      if (surface.GetType() == GeomAbs_Plane)
	{
	  McCAD::Decomposition::BoundSurfacePlane* boundSurfacePlane;
	  boundSurfacePlane->accessImpl()->initiate(face);
	  boundSurfacePlane->setSurfaceType("Plane");
	  boundSurfacePlane->accessImpl()->generateExtPlane(boxSquareLength);

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

void
McCAD::Decomposition::DecomposeSolid::Impl::generateEdges(McCAD::Decomposition::BoundSurface* boundSurface){
}

void
McCAD::Decomposition::DecomposeSolid::Impl::mergeSurfaces(std::vector<McCAD::Decomposition::BoundSurface*> planesList){
}
