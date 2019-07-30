// McCAD                                                                             
#include "splitsurfaces_impl.hpp"

Standard_Boolean
McCAD::Decomposition::SplitSurfaces::Impl::throughBoundarySurfaces(std::vector<std::shared_ptr<McCAD::Decomposition::BoundSurface>>& facesList){
  if (facesList.empty())
    {
      return Standard_False;
    }
  for (Standard_Integer i = 0; i <= facesList.size() - 1; ++i)
    {
      if (facesList[i]->accessSImpl()->numberCollidingSurfaces == 0)
	{
	  return Standard_False;
	}
    }
  return Standard_True;
}

Standard_Boolean
McCAD::Decomposition::SplitSurfaces::Impl::planeSplitOnPlane(std::vector<std::shared_ptr<McCAD::Decomposition::BoundSurface>>& facesList){
  return Standard_False;
}
