// McCAD                                                                             
#include "splitsurfaces_impl.hpp"

Standard_Boolean
McCAD::Decomposition::SplitSurfaces::Impl::throughBoundarySurfaces(std::vector<std::shared_ptr<McCAD::Decomposition::BoundSurface>>& facesList){
  if (facesList.empty())
    {
      return Standard_True;
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
McCAD::Decomposition::SplitSurfaces::Impl::planeSplitOnlyPlane(std::vector<std::shared_ptr<McCAD::Decomposition::BoundSurface>>& facesList){
  if (facesList.empty())
    {
      return Standard_False;
    }
  for (Standard_Integer i = 0; i <= facesList.size() - 1; ++i)
    {
      if (facesList[i]->getSurfaceType() == "Plane" && facesList[i]->accessSImpl()->numberCollidingCurvedSurfaces == 0)
	{
	  return Standard_True;
	}
    }

  return Standard_False;
}
