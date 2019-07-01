// McCAD
#include "decomposeSolid_impl.hpp"
#include "tools_impl.hpp"

void
McCAD::Decomposition::DecomposeSolid::Impl::perform(const TopoDS_Solid& solid, const Standard_Real& meshDeflection, Standard_Integer recursivityLevel){
  // The function will be called recursively on a solid and a condition has to be set for termination.
  // Increment the level by 1.
  recursivityLevel++;
  std::cout << "Level: " << recursivityLevel << std::endl;
  // Generate the boundary surface list of the solid.
  genSurfaceList(solid);
  // Judge which surfaces are decompose surfaces from the generated list.
  judgeDecomposeSurfaces();
  
}

void
McCAD::Decomposition::DecomposeSolid::Impl::genSurfaceList(const TopoDS_Solid& solid){
  // Generate a list of faces of the solid.
  McCAD::Tools::Preprocessor preproc;
  TopoDS_Face face;
  std::vector<TopoDS_Face> planesList;
  std::vector<TopoDS_Face> cylindersList;
  std::vector<TopoDS_Face> conesList;
  std::vector<TopoDS_Face> facesList;
  TopExp_Explorer explorer(solid, TopAbs_FACE);
  for (; explorer.More(); explorer.Next())
    {
      face = TopoDS::Face(explorer.Current());
      BRepTools::Update(face);
      Standard_Boolean rejectCondition = preproc.accessImpl()->checkFace(face);
      if (!rejectCondition)
	{
	  //
	}
      else continue;
    }

  
}

void
McCAD::Decomposition::DecomposeSolid::Impl::judgeDecomposeSurfaces(){
}
