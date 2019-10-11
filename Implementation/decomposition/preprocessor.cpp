// McCAD
#include "preprocessor.hpp"


Standard_Boolean
McCAD::Tools::Preprocessor::Impl::checkBndSurfaces(const TopoDS_Solid& solid,
						   Standard_Boolean& isTorus,
						   Standard_Boolean& isSpline){
  for(const auto& face : ShapeView<TopAbs_FACE>{solid})
    {
      GeomAdaptor_Surface surfAdaptor(BRep_Tool::Surface(face));

      if(surfAdaptor.GetType() == GeomAbs_Torus)
        {
          std::cout << "    -- The current verion doesn't support processing of Tori. Ignoring solid!" << std::endl;
          reutrn Standard_True;
        }
      else if (surfAdaptor.GetType() == GeomAbs_BSplineSurface)
        {
          std::cout << "    -- The current verion doesn't support processing of splines. Ignoring solid!" << std::endl;
          return Standard_True;
        }
    }
}

template <typename SolidType>
SolidType
McCAD::Decomposition::DecomposeSolid::Impl::perform(TopoDS_Shape& shape){
  
    // Calculate OBB of the solid.
    solidImpl.createOBB();
    // Calculate mesh deflection of the solid.
    solidImpl.calcMeshDeflection();
    // Update edges convexity of the solid.
    solidImpl.updateEdgesConvexity();
    // Generate the boundary surfaces list of the solid.
    solidImpl.generateSurfacesList();
    // Judge which surfaces are decompose surfaces from the generated list.
