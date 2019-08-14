// McCAD                                                                             
#include "splitsurfaces_impl.hpp"

Standard_Boolean
McCAD::Decomposition::SplitSurfaces::Impl::throughNoBoundarySurfaces(std::vector<std::shared_ptr<BoundSurface>>& facesList){
  if (facesList.empty())
    {
      return Standard_False;
    }
  for (Standard_Integer i = 0; i <= facesList.size() - 1; ++i)
    {
      if (facesList[i]->accessSImpl()->numberCollidingSurfaces == 0)
	{
	  return Standard_True;
	}
    }
  return Standard_False;
}

Standard_Boolean
McCAD::Decomposition::SplitSurfaces::Impl::planeSplitOnlyPlane(std::vector<std::shared_ptr<BoundSurface>>& facesList){
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

void
McCAD::Decomposition::SplitSurfaces::Impl::generateSplitFacesList(std::vector<std::shared_ptr<BoundSurface>>& splitFacesList, std::vector<std::shared_ptr<BoundSurface>>& selectedSplitFacesList){
  // 1st step: Select surfaces that go through 0 boundary surfaces.
  for (Standard_Integer i = 0; i <= splitFacesList.size() - 1; ++i)
    {
      if (splitFacesList[i]->accessSImpl()->numberCollidingSurfaces == 0)
	{
	  selectedSplitFacesList.push_back(splitFacesList[i]);
	}
    }
  if (!selectedSplitFacesList.empty())
    {
      sortSplitFaces(selectedSplitFacesList);
      return;
    }

  // 2nd step: select surfaces that go through 0 curved surfaces.
  for (Standard_Integer i = 0; i <= splitFacesList.size() - 1; ++i)
    {
      if (splitFacesList[i]->accessSImpl()->numberCollidingCurvedSurfaces == 0 && splitFacesList[i]->accessSImpl()->surfaceType == "Plane")
        {
          selectedSplitFacesList.push_back(splitFacesList[i]);
        }
    }
  if (!selectedSplitFacesList.empty())
    {
      sortSplitFaces(selectedSplitFacesList);
      return;
    }

  // 3th step: for cylinders

  // 4th step: select the other surfaces
  for (Standard_Integer i = 0; i <= splitFacesList.size() - 1; ++i)
    {
      if (splitFacesList[i]->accessSImpl()->surfaceType == "Plane")
	{
	  selectedSplitFacesList.push_back(splitFacesList[i]);
	}
    }
  if (!selectedSplitFacesList.empty())
    {
      sortSplitFaces(selectedSplitFacesList);
    }
}

void
McCAD::Decomposition::SplitSurfaces::Impl::sortSplitFaces(std::vector<std::shared_ptr<BoundSurface>>& splitFacesList){
  if (splitFacesList.size() <= 1)
    {
      return;
    }
  // Implement more modern sorting. maybe utilizing sorting of zipped vectors.
  // A Splitting surface that goes through more concave edges has priority.
  for (Standard_Integer i = 0; i <= splitFacesList.size() - 2; ++i)
    {
      for (Standard_Integer j = i + 1; j <= splitFacesList.size() - 1; ++j)
	{
	  if (splitFacesList[i]->accessSImpl()->throughConcaveEdges < splitFacesList[j]->accessSImpl()->throughConcaveEdges)
	    {
	      std::swap(splitFacesList[i], splitFacesList[j]);
	    }
	}
    }
  // A Splitting surface that intesects with less boundary surfaces has priority.
  for (Standard_Integer i = 0; i <= splitFacesList.size() - 2; ++i)
    {
      for (Standard_Integer j = i + 1; j <= splitFacesList.size() - 1; ++j)
        {
	  if (splitFacesList[i]->accessSImpl()->throughConcaveEdges <= splitFacesList[j]->accessSImpl()->throughConcaveEdges)
	    {
	      if (splitFacesList[i]->accessSImpl()->numberCollidingSurfaces > splitFacesList[j]->accessSImpl()->numberCollidingSurfaces)
		{
		  std::swap(splitFacesList[i], splitFacesList[j]);
		}
	    }
	}
    }
  /*
  for (Standard_Integer i = 0; i <= splitFacesList.size() - 1; ++i)
    {
      //std::cout << "throughConcaveEdges: " << splitFacesList[i]->accessSImpl()->throughConcaveEdges << " , numberCollidingSurfaces: " << splitFacesList[i]->accessSImpl()->numberCollidingSurfaces << std::endl;
    }
  */
}
