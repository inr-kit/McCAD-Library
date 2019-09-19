// McCAD
#include "decomposeSolid_impl.hpp"

McCAD::Decomposition::DecomposeSolid::Impl::Impl(){
}

McCAD::Decomposition::DecomposeSolid::Impl::~Impl(){
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::perform(std::unique_ptr<Geometry::Solid>& solid){
  // The function will be called recursively on a solid and a condition has to be set on the recurrence level for termination.
  ++recurrenceDepth;
  std::cout << "     - Recurrence Depth: " << recurrenceDepth << std::endl;
  auto solidImpl = solid->accessSImpl();
  // Create OBB and calculate mesh deflection of the solid.
  solidImpl->createOBB();
  solidImpl->calcMeshDeflection();
  // Update edges convexity of the solid.
  solidImpl->updateEdgesConvexity();
  // Generate a list of the boundary surfaces of the solid.
  solidImpl->generateSurfacesList();
  // Judge which surfaces are decompose surfaces from the generated list.
  judgeDecomposeSurfaces(solidImpl->facesList, solidImpl->splitFacesList,
			 solidImpl->splitSurface);
  if (!splitSurfaces.accessSSImpl()->throughNoBoundarySurfaces(solidImpl->splitFacesList))
    {
      //std::cout << "throughNoBoundarySurfaces false" << std::endl;
      judgeThroughConcaveEdges(solidImpl->splitFacesList);
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
  
  if (solidImpl->splitSurface)
    {
      //std::cout << "Solid has a split surface" << std::endl;
      if (!selectSplitSurface(solid))
	{
	  //std::cout << "** selectSplitSurface fail" << std::endl;
	  return Standard_False;
	}
      // Split the solid with the selected surface.
      //std::cout << "selected surface concave edges: " << selectedSplitFacesList[0]->accessSImpl()->throughConcaveEdges << std::endl;
      if (!splitSolid.accessSSImpl()->perform(solid))
	{
	  /*
          STEPControl_Writer writer4;
	  writer4.Transfer(solidImpl->selectedSplitFacesList[0]->accessSImpl()->extendedFace, STEPControl_StepModelType::STEPControl_AsIs);
	  writer4.Write("../examples/bbox/splitSurface.stp");
	  std::cout << "** splitSolid fail" << std::endl;
          */
          return Standard_False;
	}
      // Loop over the resulting subsolids and split each one of them recursively.
      //throw std::runtime_error {"end"};
      for (Standard_Integer i = 1; i <= solidImpl->splitSolidList->Length(); ++i)
	{
	  std::cout << "   - Decomposing subsolid # " << recurrenceDepth << "/" << solidImpl->splitSolidList->Length() << "/" << i << std::endl;
	  std::unique_ptr<Geometry::Solid> subSolid = std::make_unique<Geometry::Solid>();
	  auto subSolidImpl = subSolid->accessSImpl();
	  try
	    {
	      subSolidImpl->initiate(solidImpl->splitSolidList->Value(i));
	    }
	  catch(const Standard_ConstructionError&)
	    {
	      /*
	      std::cout << "** subSolidImpl->initiate fail" << std::endl;
	      STEPControl_Writer writer3;
	      writer3.Transfer(solidImpl->splitSolidList->Value(i), STEPControl_StepModelType::STEPControl_AsIs);
	      writer3.Write("../examples/bbox/subsolid.stp");
	      STEPControl_Writer writer4;
	      writer4.Transfer(solidImpl->selectedSplitFacesList[0]->accessSImpl()->extendedFace, STEPControl_StepModelType::STEPControl_AsIs);
	      writer4.Write("../examples/bbox/splitsurface)subsolid.stp");
	      throw std::runtime_error{"Shape problem"};
	      */
	      return Standard_False;
	    }
	  std::unique_ptr<DecomposeSolid> decomposeSolid = std::make_unique<DecomposeSolid>();
	  decomposeSolid->accessDSImpl()->recurrenceDepth = recurrenceDepth;
	  // mesh deflection is calculated inside initiate for every solid!.
          if (decomposeSolid->accessDSImpl()->perform(subSolid))
            {
	      if (subSolidImpl->splitSolidList->Length() >= 2)
		{
		  for (const auto& resultSubSolid : *subSolidImpl->splitSolidList)
		    {
		      solidImpl->splitSolidList->InsertAfter(i, resultSubSolid);
		    }
		  solidImpl->splitSolidList->Remove(i);
		  i += subSolidImpl->splitSolidList->Length() - 1;
		}
	      // Add rejected subSolids
	      if (subSolidImpl->rejectedsubSolidsList->Length() >= 1)
		{
		  for (const auto& rejectedSubSolid : *subSolidImpl->rejectedsubSolidsList)
		    {
		      solidImpl->rejectedsubSolidsList->Append(rejectedSubSolid);
		    }
		}
	      //return Standard_True;
	    }
	  else
	    {
	      //std::cout << "** decompose subsolid fail" << std::endl;
	      //return Standard_False;
	      solidImpl->rejectedsubSolidsList->Append(solidImpl->splitSolidList->Value(i));
	    }
	}
      //return Standard_True;
    }
  else
    {
      //std::cout	<< "Solid has no split surfaces" << std::endl;
      solidImpl->splitSolidList->Append(solidImpl->solid);
    }
  return Standard_True;
}

void
McCAD::Decomposition::DecomposeSolid::Impl::judgeDecomposeSurfaces(std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList,
								   std::vector<std::shared_ptr<Geometry::BoundSurface>>& splitFacesList,
								   Standard_Boolean& splitSurface){
  // Judge whether boundary surfaces of the solid can be used for decomposition.
  //std::cout << "judgeDecomposeSurfaces" << std::endl;
  if (facesList.size() < 2)
    {
      return;
    }
  //std::cout << "facesList.size(): " << facesList.size() << std::endl;
  for (Standard_Integer i = 0; i <= facesList.size() - 1; ++i)
    {
      //std::cout << "judge: " << i << std::endl;;
      Standard_Integer positiveFaces = 0;
      Standard_Integer negativeFaces = 0;
      Standard_Integer numberCollidingSurfaces = 0;
      Standard_Integer numberCollidingCurvedSurfaces = 0;
      for (Standard_Integer j = 0; j <= facesList.size() - 1; ++j)
	{
	  //std::cout << "judge:    " << j << std::endl;
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
		  //std::cout << "side" << side << std::endl;
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
	  splitFacesList.push_back(facesList[i]);
	  splitSurface = Standard_True;
	}
    }
}


void
McCAD::Decomposition::DecomposeSolid::Impl::judgeThroughConcaveEdges(std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList){
  // Judge how many concave edges each boundary face of solid goes through.
  if (facesList.size() == 0)
    {
      //std::cout << "return" << std::endl;
      return;
    }
  for (Standard_Integer i = 0; i <= facesList.size() - 1; ++i)
    {
      if (facesList[i]->accessSImpl()->throughConcaveEdges != 0) continue;
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
McCAD::Decomposition::DecomposeSolid::Impl::selectSplitSurface(std::unique_ptr<Geometry::Solid>& solid){
  // mergeSplitSurfaces(splitFacesList) // see no need for it as the formed lists; planesList, etc. are already merged. splitFacesList is a subset of facesList.
  splitSurfaces.accessSSImpl()->generateSplitFacesList(solid->accessSImpl()->splitFacesList, solid->accessSImpl()->selectedSplitFacesList);
  if (!(solid->accessSImpl()->selectedSplitFacesList.empty()))
    {
      return Standard_True;
    }
  return Standard_False;
}
