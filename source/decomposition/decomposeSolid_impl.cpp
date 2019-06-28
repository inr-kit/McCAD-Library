// McCAD
#include "decomposeSolid_impl.hpp"

void
McCAD::Decomposition::DecomposeSolid::Impl::perform(const TopoDS_Solid& solid, const Standard_Integer& meshDeflection){
  // The function will be called recursively on a solid and a condition has to be set to terminate.
  // Increment the level by 1.
  recursivityLevel++;
  // Generate the boundary surface list of the solid.
  genSurfaceList(solid);
  // Judge which surfaces are decompose surfaces
  //judgeDecomposeSurface();
}

void
McCAD::Decomposition::DecomposeSolid::Impl::genSurfaceList(const TopoDS_Solid& solid){
  // Generate a list of faces of the solid.
  TopoDS_Face face;
  TopExp_Explorer explorer(solid, TopAbs_FACE);
  for (; explorer.More(); explorer.Next())
    {
      face = TopoDS::Face(explorer.Current());
      BRepTools::Update(face);
    }
      

}
