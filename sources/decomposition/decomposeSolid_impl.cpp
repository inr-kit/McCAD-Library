// McCAD
#include "decomposeSolid_impl.hpp"

McCAD::Decomposition::DecomposeSolid::Impl::Impl() : splitSolidList{std::make_unique<TopTools_HSequenceOfShape>()}, rejectedsubSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}{
}

McCAD::Decomposition::DecomposeSolid::Impl::~Impl(){
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::perform(Solid& solid){
  // The function will be called recursively on a solid and a condition has to be set for termination.
  // Increment the recurrence depth by 1.
  ++recurrenceDepth;
  std::cout << "     - Recurrence Depth: " << recurrenceDepth << std::endl;
  // Update edges convexity of the solid.
  solid->accessSImpl()->updateEdgesConvexity();
  //throw std::runtime_error("saved concave edges");
  // Generate the boundary surfaces list of the solid.
  solid->accessSImpl()->generateSurfacesList();
  /*
  STEPControl_Writer wrt;
  for (Standard_Integer DD = 0; DD <= facesList.size() - 1; ++DD)
    {
      wrt.Transfer(facesList[DD]->accessSImpl()->face, STEPControl_AsIs);
    }
  std::ostringstream oss;
  oss << "./run/" << recurrenceDepth << "_faceslist" << ".stp";
  std::string listname = oss.str();
  wrt.Write(listname.c_str());
  */
  //throw std::runtime_error("saved facesList");

  // Judge which surfaces are decompose surfaces from the generated list.
  judgeDecomposeSurfaces(solid);
  //std::cout << "length of faceslist: " << facesList.size() << std::endl;
  //std::cout << "length of splitfaceslist: " << splitFacesList.size() << std::endl;
  //std::cout << "splitSurface: " << splitSurface  << std::endl;
  /*
  if (recurrenceDepth == 3)
    {
      STEPControl_Writer wrt200;
      wrt200.Transfer(solid, STEPControl_AsIs);
      std::ostringstream oss200;
      oss200 << "./run/" << recurrenceDepth <<  "_solid.stp";
      std::string solidname00 = oss200.str();
      wrt200.Write(solidname00.c_str());
      //throw std::runtime_error("odd surfaces");
    }
  */

  //judgeThroughConcaveEdges(splitFacesList);
  if (!splitSurfaces.accessSSImpl()->throughNoBoundarySurfaces(solid->accessSImpl()->splitFacesList))
    {
      //std::cout << "throughNoBoundarySurfaces false" << std::endl;
      judgeThroughConcaveEdges(solid->accessSImpl()->splitFacesList);
      /*
      if (!splitSurfaces.accessSSImpl()->planeSplitOnlyPlane(splitFacesList))
	{
	  //generateAssistingSurfaces();
	  //judgeAssistingDecomposeSurfaces();
	  //judgeThroughConcaveEdges(assistingFacesList);
	}
      */
    }
  //Standard_Integer selectedSurface = 0;
  
  if (recurrenceDepth >= 20)
    {
      return Standard_False;
    }
  
  if (solid->accessSImpl()->splitSurface)
    {
      //std::cout << "Solid has a split surface" << std::endl;
      if (!selectSplitSurface())
	{
	  return Standard_False;
	}
      // Split the solid with the selected surface.
      //std::cout << "selected surface concave edges: " << selectedSplitFacesList[0]->accessSImpl()->throughConcaveEdges << std::endl;
      /*
      STEPControl_Writer wrt;
      wrt.Transfer(selectedSplitFacesList[0]->accessSImpl()->extendedFace, STEPControl_AsIs);
      std::ostringstream oss;
      oss << "./run/" << recurrenceDepth << "_splitFace.stp";
      std::string surfname = oss.str();
      wrt.Write(surfname.c_str());
      */
      if (!splitSolid.accessSSImpl()->initiate(solid, selectedSplitFacesList[0], splitSolidList))
	{
	  return Standard_False;
	}

      /*
      STEPControl_Writer wrt2;
      for (Standard_Integer DD = 1; DD <= splitSolidList->Length(); ++DD)
	{
	  wrt2.Transfer(splitSolidList->Value(DD), STEPControl_AsIs);
	}
      std::ostringstream oss2;
      oss2 << "./run/" << recurrenceDepth << "_splitsolidslist.stp";
      std::string solidname = oss2.str();
      wrt2.Write(solidname.c_str());
      */
      // Loop over the resulting subsolids and split each one of them recursively.
      //std::cout << "splitting subsolids" << std::endl;
      for (Standard_Integer i = 1; i <= splitSolidList->Length(); ++i)
	{
	  std::cout << "   - Decomposing subsolid # " << recurrenceDepth << "/" << splitSolidList->Length() << "/" << i << std::endl;
	  //std::cout << splitSolidList->Length() << std::endl;
	  TopoDS_Solid subSolid;
	  try
	    {
	      subSolid = TopoDS::Solid(splitSolidList->Value(i));
	    }
	  catch(...)
	    {
	      return Standard_False;
	    }
	  std::unique_ptr<DecomposeSolid> decomposedSolid = std::make_unique<DecomposeSolid>();
	  decomposedSolid->accessDSImpl()->recurrenceDepth = recurrenceDepth;
	  // mesh deflection is calculated inside initiate for every solid!.
          if (decomposedSolid->accessDSImpl()->initiate(subSolid))
            {
	      if (decomposedSolid->accessDSImpl()->splitSolidList->Length() >= 2)
		{
		  //splitSolidList->Remove(i);
		  for (Standard_Integer j = 1; j <= decomposedSolid->accessDSImpl()->splitSolidList->Length(); ++j)
		    {
		      splitSolidList->InsertAfter(i, decomposedSolid->accessDSImpl()->splitSolidList->Value(j));
		    }
		  splitSolidList->Remove(i);
		  i += decomposedSolid->accessDSImpl()->splitSolidList->Length() - 1;
		  // Add rejected subSolids
		}
	      if (decomposedSolid->accessDSImpl()->rejectedsubSolidsList->Length() >= 1)
		{
		  for (Standard_Integer j = 1; j <= decomposedSolid->accessDSImpl()->rejectedsubSolidsList->Length(); ++j)
		    {
		      rejectedsubSolidsList->Append(decomposedSolid->accessDSImpl()->rejectedsubSolidsList->Value(j));
		    }
		}
	      //return Standard_True;
	    }
	  else
	    {
	      //return Standard_False;
	      rejectedsubSolidsList->Append(subSolid);
	    }
	}
      //return Standard_True;
    }
  else
    {
      //std::cout	<< "Solid has no split surfaces" << std::endl;
      splitSolidList->Append(solid);
    }
  return Standard_True;
}

void
McCAD::Decomposition::DecomposeSolid::Impl::judgeDecomposeSurfaces(Solid& solid){
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
	  if (i != j && facesList[i]->accessSImpl()->surfaceNumber != facesList[j]->accessSImpl()->surfaceNumber)
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
McCAD::Decomposition::DecomposeSolid::Impl::selectSplitSurface(Solid& solid){
  // mergeSplitSurfaces(splitFacesList) // see no need for it as the formed lists; planesList, etc. are already merged. splitFacesList is a subset of facesList.
  splitSurfaces.accessSSImpl()->generateSplitFacesList(solid->accessSImpspl()->itFacesList, solid->accessSImpl()->selectedSplitFacesList);
  if (!solid->accessSImpl()->selectedSplitFacesList.empty())
    {
      return Standard_True;
    }
  return Standard_False;
}
