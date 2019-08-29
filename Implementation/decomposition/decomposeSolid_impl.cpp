// McCAD
#include "decomposeSolid_impl.hpp"

McCAD::Decomposition::DecomposeSolid::Impl::Impl(){
}

McCAD::Decomposition::DecomposeSolid::Impl::~Impl(){
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::perform(std::unique_ptr<Solid>& solid){
  // The function will be called recursively on a solid and a condition has to be set for termination.
  // Increment the recurrence depth by 1.
  ++recurrenceDepth;
  std::cout << "     - Recurrence Depth: " << recurrenceDepth << std::endl;
  // Alias the access to the implementation of solid object.
  auto solid_impl = solid->accessSImpl();
  // Calculate mesh deflection of the solid.
  solid_impl->calcMeshDeflection();
  // Update edges convexity of the solid.
  solid_impl->updateEdgesConvexity();
  // Generate the boundary surfaces list of the solid.
  solid_impl->generateSurfacesList();
  // Judge which surfaces are decompose surfaces from the generated list.
  judgeDecomposeSurfaces(solid);
  //judgeThroughConcaveEdges(splitFacesList);
  if (!splitSurfaces.accessSSImpl()->throughNoBoundarySurfaces(solid_impl->splitFacesList))
    {
      //std::cout << "throughNoBoundarySurfaces false" << std::endl;
      judgeThroughConcaveEdges(solid_impl->splitFacesList);
      /*
      if (!splitSurfaces.accessSSImpl()->planeSplitOnlyPlane(splitFacesList))
	{
	  //generateAssistingSurfaces();
	  //judgeAssistingDecomposeSurfaces();
	  //judgeThroughConcaveEdges(assistingFacesList);
	}
      */
    }
  
  if (recurrenceDepth >= 20)
    {
      return Standard_False;
    }
  
  if (solid_impl->splitSurface)
    {
      //std::cout << "Solid has a split surface" << std::endl;
      if (!selectSplitSurface(solid))
	{
	  return Standard_False;
	}
      // Split the solid with the selected surface.
      //std::cout << "selected surface concave edges: " << selectedSplitFacesList[0]->accessSImpl()->throughConcaveEdges << std::endl;
      if (!(splitSolid.accessSSImpl()->perform(solid_impl->solid, solid_impl->selectedSplitFacesList[0], solid_impl->splitSolidList)))
	{
	  //std::cout << "return of splitsolid" << std::endl;
	  return Standard_False;
	}
      // Loop over the resulting subsolids and split each one of them recursively.
      //std::cout << "splitting subsolids" << std::endl;
      for (Standard_Integer i = 1; i <= solid_impl->splitSolidList->Length(); ++i)
	{
	  std::cout << "   - Decomposing subsolid # " << recurrenceDepth << "/" << solid_impl->splitSolidList->Length() << "/" << i << std::endl;
	  //std::cout << splitSolidList->Length() << std::endl;
	  std::unique_ptr<Solid> subSolid = std::make_unique<Solid>();
	  auto subSolid_impl = subSolid->accessSImpl();
	  try
	    {
	      subSolid_impl->initiate(solid_impl->splitSolidList->Value(i));
	    }
	  catch(...)
	    {
	      return Standard_False;
	    }
	  std::unique_ptr<DecomposeSolid> decomposeSolid = std::make_unique<DecomposeSolid>();
	  decomposeSolid->accessDSImpl()->recurrenceDepth = recurrenceDepth;
	  // mesh deflection is calculated inside initiate for every solid!.
          if (decomposeSolid->accessDSImpl()->perform(subSolid))
            {
	      if (subSolid_impl->splitSolidList->Length() >= 2)
		{
		  //splitSolidList->Remove(i);
		  for (Standard_Integer j = 1; j <= subSolid_impl->splitSolidList->Length(); ++j)
		    {
		      solid_impl->splitSolidList->InsertAfter(i, subSolid_impl->splitSolidList->Value(j));
		    }
		  solid_impl->splitSolidList->Remove(i);
		  i += subSolid_impl->splitSolidList->Length() - 1;
		}
	      // Add rejected subSolids
	      if (subSolid_impl->rejectedsubSolidsList->Length() >= 1)
		{
		  for (Standard_Integer j = 1; j <= subSolid_impl->rejectedsubSolidsList->Length(); ++j)
		    {
		      solid_impl->rejectedsubSolidsList->Append(subSolid_impl->rejectedsubSolidsList->Value(j));
		    }
		}
	      //return Standard_True;
	    }
	  else
	    {
	      //return Standard_False;
	      solid_impl->rejectedsubSolidsList->Append(subSolid_impl->solid);
	    }
	}
      //return Standard_True;
    }
  else
    {
      //std::cout	<< "Solid has no split surfaces" << std::endl;
      solid_impl->splitSolidList->Append(solid_impl->solid);
    }
  return Standard_True;
}

void
McCAD::Decomposition::DecomposeSolid::Impl::judgeDecomposeSurfaces(std::unique_ptr<Solid>& solid){
  // Judge whether boundary surfaces of the solid can be used for decomposition.
  //std::cout << "judgeDecomposeSurfaces" << std::endl;
  auto& facesList = solid->accessSImpl()->facesList;
  if (facesList.size() < 2)
    {
      return;
    }
  for (Standard_Integer i = 0; i <= facesList.size() - 1; ++i)
    {
      //std::cout << "judge: " << i << " , " << std::endl;
      Standard_Integer positiveFaces = 0;
      Standard_Integer negativeFaces = 0;
      Standard_Integer numberCollidingSurfaces = 0;
      Standard_Integer numberCollidingCurvedSurfaces = 0;
      for (Standard_Integer j = 0; j <= facesList.size() - 1; ++j)
	{
	  //std::cout << "judge: " << j << " , " << std::endl;
	  if (i != j && facesList[i]->accessSImpl()->surfaceNumber !=
	      facesList[j]->accessSImpl()->surfaceNumber)
	    {
	      Standard_Integer side = 0;
	      if (facesList[i]->accessBSImpl()->faceCollision(*(facesList[j]), side))
		{
		  ++numberCollidingSurfaces;
		  //std::cout << "facecollision True" << std::endl;
		  facesList[i]->accessSImpl()->splitSurface = Standard_True;
		  if (facesList[j]->getSurfaceType() != "Plane")
		    {
		      ++numberCollidingCurvedSurfaces;
		    }
		}
	      else
		{
		  //std::cout << "facecollision False" << std::endl;
		  //std::cout << side << std::endl;
		  if (side == 1)
		    {
		      ++positiveFaces;
		    }
		  else if (side == -1)
		    {
		      ++negativeFaces;
		    }
		}
	    }
	}
      if (positiveFaces > 0 && negativeFaces > 0)
	{
	  //std::cout << "splitsutface True, pos & neg" << std::endl;
	  facesList[i]->accessSImpl()->splitSurface = Standard_True;
	}
      if (facesList[i]->accessSImpl()->splitSurface)
	{
	  //std::cout << "set collidingsurfaces" << std::endl;
	  facesList[i]->accessSImpl()->numberCollidingSurfaces = numberCollidingSurfaces;
	  facesList[i]->accessSImpl()->numberCollidingCurvedSurfaces = numberCollidingCurvedSurfaces;
	  //std::cout << "adding to split surfaces list" << std::endl;
	  solid->accessSImpl()->splitFacesList.push_back(facesList[i]);
	  solid->accessSImpl()->splitSurface = Standard_True;
	}
    }
}

void
McCAD::Decomposition::DecomposeSolid::Impl::judgeThroughConcaveEdges(std::vector<std::shared_ptr<BoundSurface>>& facesList){
  // Judge how many concave edges each boundary face of solid goes through.
  if (facesList.size() == 0)
    {
      //std::cout << "return" << std::endl;
      return;
    }
  for (Standard_Integer i = 0; i <= facesList.size() - 1; ++i)
    {
      Standard_Integer throughConcaveEdges = 0;
      for (Standard_Integer j = 0; j <= facesList.size() - 1; ++j)
	{
	  if (i != j && facesList[i]->accessSImpl()->surfaceNumber != facesList[j]->accessSImpl()->surfaceNumber)
	    {
	      //std::cout << "checking edges" << std::endl;
	      for (Standard_Integer k = 0; k <= facesList[j]->accessBSImpl()->edgesList.size() - 1; ++k)
		{
		  if (facesList[j]->accessBSImpl()->edgesList[k]->accessEImpl()->convexity == 0 && facesList[i]->accessBSImpl()->edgeOnSurface(*(facesList[j]->accessBSImpl()->edgesList[k])))
		    {
		      ++throughConcaveEdges;
		    }
		}
	    }
	}
      facesList[i]->accessSImpl()->throughConcaveEdges = throughConcaveEdges;
      //std::cout << "throughConcaveEdges: " << throughConcaveEdges << std::endl;
    }
}

void
McCAD::Decomposition::DecomposeSolid::Impl::generateAssistingSurfaces(){
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::selectSplitSurface(std::unique_ptr<Solid>& solid){
  // mergeSplitSurfaces(splitFacesList) // see no need for it as the formed lists; planesList, etc. are already merged. splitFacesList is a subset of facesList.
  splitSurfaces.accessSSImpl()->generateSplitFacesList(solid->accessSImpl()->splitFacesList, solid->accessSImpl()->selectedSplitFacesList);
  if (!(solid->accessSImpl()->selectedSplitFacesList.empty()))
    {
      return Standard_True;
    }
  return Standard_False;
}
