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
	  //std::cout << "faceNumber: " << faceNumber << std::endl;
	  preproc.accessImpl()->fixFace(face);
	  //std::cout << "face fixed: " << std::endl;
	  std::unique_ptr<McCAD::Decomposition::BoundSurface> boundSurface = std::move(generateSurface(face));
	  boundSurface->accessSImpl()->initiate(face);
	  boundSurface->accessSImpl()->surfaceNumber = faceNumber;
	  if (boundSurface->accessBSImpl()->generateMesh(meshDeflection))
	    {
	      generateEdges(boundSurface);
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
	  std::cout << getSurfTypeName(AdaptorSurface.GetType()) << std::endl;
	  std::unique_ptr<McCAD::Decomposition::BoundSurfacePlane> boundSurfacePlane = std::make_unique<McCAD::Decomposition::BoundSurfacePlane>();
	  boundSurfacePlane->setSurfaceType(boundSurfacePlane->accessBSPImpl()->surfaceType);
	  boundSurfacePlane->accessBSPImpl()->generateExtPlane(boxSquareLength);
	  //assert(boundSurfacePlane);
	  //std::cout << "return poly" << std::endl;
	  return boundSurfacePlane;
	}
      else if (AdaptorSurface.GetType() == GeomAbs_Cylinder)
	{
	  std::cout << getSurfTypeName(AdaptorSurface.GetType()) << std::endl;
	  std::unique_ptr<McCAD::Decomposition::BoundSurface> boundSurfaceCylinder = std::make_unique<McCAD::Decomposition::BoundSurface>();
	  return boundSurfaceCylinder;
	}
      else if (AdaptorSurface.GetType() == GeomAbs_Cone)
	{
	  std::cout << getSurfTypeName(AdaptorSurface.GetType()) << std::endl;
	  std::unique_ptr<McCAD::Decomposition::BoundSurface> boundSurfaceCone = std::make_unique<McCAD::Decomposition::BoundSurface>();
	  return boundSurfaceCone;
	}
      else
	{
	  std::cout << getSurfTypeName(AdaptorSurface.GetType()) << std::endl;
	  std::unique_ptr<McCAD::Decomposition::BoundSurface> boundSurface = std::make_unique<McCAD::Decomposition::BoundSurface>();
          return boundSurface;
	}
    }
  //std::cout << "<<<<<<<<<<<<<< mode 1" << std::endl;
  return nullptr;
}

void
McCAD::Decomposition::DecomposeSolid::Impl::judgeDecomposeSurfaces(){
}

void
McCAD::Decomposition::DecomposeSolid::Impl::generateEdges(const std::unique_ptr<McCAD::Decomposition::BoundSurface>& surface){
}

void
McCAD::Decomposition::DecomposeSolid::Impl::mergeSurfaces(const std::vector<std::unique_ptr<McCAD::Decomposition::BoundSurface>>& surfacesList){
  std::vector<std::unique_ptr<McCAD::Decomposition::BoundSurface>> mergedSurfacesList;
  std::unique_ptr<McCAD::Decomposition::BoundSurface> boundSurface1 = std::make_unique<McCAD::Decomposition::BoundSurface>();
  mergedSurfacesList.push_back(std::move(boundSurface1));
  std::cout << facesList.size() << std::endl;
  for (Standard_Integer i = 1; i <= mergedSurfacesList.size(); ++i)
    {
      facesList.push_back(std::move( mergedSurfacesList[i-1]));
    }
  std::cout << facesList.size() << std::endl;
}

std::string
McCAD::Decomposition::DecomposeSolid::Impl::getSurfTypeName(const Standard_Integer& index){
  // This function is to be deleted and has to write a tool to get the type from the enumerated type from OCE rather than copy the types into a vector as is done here!.
  std::vector<std::string> typeNames = {
	"Plane",
	"Cylinder",
	"Cone",
	"Sphere",
	"Torus",
	"BezierSurface",
	"BSplineSurface",
	"SurfaceOfRevolution",
	"SurfaceOfExtrusion",
	"OffsetSurface",
	"OtherSurface"
	};
  return typeNames[index];
}
