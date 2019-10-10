// McCAD
#include "decomposeSolid_impl.hpp"

McCAD::Decomposition::DecomposeSolid::Impl::Impl()
    : recurrenceDepth{0}{
}

McCAD::Decomposition::DecomposeSolid::Impl::Impl(
        Standard_Integer recurrenceDepth)
    : recurrenceDepth{recurrenceDepth}{
}

McCAD::Decomposition::DecomposeSolid::Impl::~Impl(){
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::perform(
        Geometry::Solid::Impl& solidImpl){
    // The function will be called recursively on a solid and a condition has to be set for termination.
    // Increment the recurrence depth by 1.
    ++recurrenceDepth;
    std::cout << "     - Recurrence Depth: " << recurrenceDepth << std::endl;
    // Alias the access to the implementation of solid object.
    // Calculate mesh deflection of the solid.
    solidImpl.calcMeshDeflection();
    // Update edges convexity of the solid.
    solidImpl.updateEdgesConvexity();
    // Generate the boundary surfaces list of the solid.
    solidImpl.generateSurfacesList();
    // Judge which surfaces are decompose surfaces from the generated list.
    judgeDecomposeSurfaces(solidImpl);
    //judgeThroughConcaveEdges(splitFacesList);
    if(!SplitSurfaces::Impl::throughNoBoundarySurfaces(solidImpl.splitFacesList)){
        //std::cout << "throughNoBoundarySurfaces false" << std::endl;
        judgeThroughConcaveEdges(solidImpl.splitFacesList);
        /*
      if (!splitSurfaces.accessSSImpl()->planeSplitOnlyPlane(splitFacesList))
	{
	  //generateAssistingSurfaces();
	  //judgeAssistingDecomposeSurfaces();
	  //judgeThroughConcaveEdges(assistingFacesList);
	}
      */
    }
  
    if(recurrenceDepth >= 20){
        return Standard_False;
    }
  
    if(solidImpl.splitSurface){
        //std::cout << "Solid has a split surface" << std::endl;
        if (!selectSplitSurface(solidImpl)){
            return Standard_False;
        }
        // Split the solid with the selected surface.
        //std::cout << "selected surface concave edges: " << selectedSplitFacesList[0]->accessSImpl()->throughConcaveEdges << std::endl;
        if(!(SplitSolid::Impl{}(
                 solidImpl.solid,
                 solidImpl.obb,
                 *solidImpl.selectedSplitFacesList[0],
                 *solidImpl.splitSolidList))){
        //std::cout << "return of splitsolid" << std::endl;
            return Standard_False;
        }
        // Loop over the resulting subsolids and split each one of them recursively.
        //std::cout << "splitting subsolids" << std::endl;

        for (Standard_Integer i = 1; i <= solidImpl.splitSolidList->Length(); ++i){

            std::cout << "   - Decomposing subsolid # "
                      << recurrenceDepth << "/"
                      << solidImpl.splitSolidList->Length()
                      << "/" << i << std::endl;
            //std::cout << splitSolidList->Length() << std::endl;

            Geometry::Solid::Impl subSolidImpl;
            try{
                subSolidImpl.initiate(solidImpl.splitSolidList->Value(i));
            }catch(...){
                return Standard_False;
            }

            // mesh deflection is calculated inside initiate for every solid!.
            if (DecomposeSolid::Impl{recurrenceDepth}(subSolidImpl)){
                if (subSolidImpl.splitSolidList->Length() >= 2){
                    //splitSolidList->Remove(i);
                    for(const auto& el : *subSolidImpl.splitSolidList)
                        solidImpl.splitSolidList->InsertAfter(i, el);

                    solidImpl.splitSolidList->Remove(i);
                    i += subSolidImpl.splitSolidList->Length() - 1;
                }
                // Add rejected subSolids
                solidImpl.rejectedsubSolidsList->Append(
                          *subSolidImpl.rejectedsubSolidsList);
                //return Standard_True;
            }else{
                //return Standard_False;
                solidImpl.rejectedsubSolidsList->Append(
                            subSolidImpl.solid);
            }
        }
        //return Standard_True;
    }else{
        //std::cout	<< "Solid has no split surfaces" << std::endl;
        solidImpl.splitSolidList->Append(solidImpl.solid);
    }
    return Standard_True;
}

bool
McCAD::Decomposition::DecomposeSolid::Impl::operator()(
        Geometry::Solid::Impl& solid){
    return perform(solid);
}

void
McCAD::Decomposition::DecomposeSolid::Impl::judgeDecomposeSurfaces(
        Geometry::Solid::Impl& solidImpl){
    // Judge whether boundary surfaces of the solid can be used for decomposition.
    //std::cout << "judgeDecomposeSurfaces" << std::endl;
    auto& facesList = solidImpl.facesList;
    if (facesList.size() < 2){
        return;
    }
    for (Standard_Integer i = 0; i < facesList.size(); ++i){
        auto& thisFace = *facesList[i]->accessSImpl();

        //std::cout << "judge: " << i << " , " << std::endl;
        Standard_Integer positiveFaces = 0;
        Standard_Integer negativeFaces = 0;
        Standard_Integer numberCollidingSurfaces = 0;
        Standard_Integer numberCollidingCurvedSurfaces = 0;
        for (Standard_Integer j = 0; j < facesList.size(); ++j){
            auto& thatFace = *facesList[j]->accessSImpl();

            //std::cout << "judge: " << j << " , " << std::endl;
            if (i != j && thisFace.surfaceNumber != thatFace.surfaceNumber){
                Standard_Integer side = 0;
                if (facesList[i]->accessBSImpl()->faceCollision(*facesList[j], side)){
                    ++numberCollidingSurfaces;
                    //std::cout << "facecollision True" << std::endl;
                    thisFace.splitSurface = Standard_True;
                    if (facesList[j]->getSurfaceType() != "Plane"){
                        ++numberCollidingCurvedSurfaces;
                    }
                }else{
                    //std::cout << "facecollision False" << std::endl;
                    //std::cout << side << std::endl;
                    if (side == 1){
                        ++positiveFaces;
                    }else if (side == -1){
                        ++negativeFaces;
                    }
                }
            }
        }

        if (positiveFaces > 0 && negativeFaces > 0){
            //std::cout << "splitsutface True, pos & neg" << std::endl;
            thisFace.splitSurface = Standard_True;
        }

        if (thisFace.splitSurface){
            //std::cout << "set collidingsurfaces" << std::endl;
            thisFace.numberCollidingSurfaces = numberCollidingSurfaces;
            thisFace.numberCollidingCurvedSurfaces = numberCollidingCurvedSurfaces;
            //std::cout << "adding to split surfaces list" << std::endl;
            solidImpl.splitFacesList.push_back(facesList[i]);
            solidImpl.splitSurface = Standard_True;
        }
    }
}

void
McCAD::Decomposition::DecomposeSolid::Impl::judgeThroughConcaveEdges(
        std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList){
    // Judge how many concave edges each boundary face of solid goes through.
    for (Standard_Integer i = 0; i < facesList.size(); ++i){
        Standard_Integer throughConcaveEdges = 0;
        for (Standard_Integer j = 0; j < facesList.size(); ++j){
            if (i != j &&
                    facesList[i]->accessSImpl()->surfaceNumber
                    != facesList[j]->accessSImpl()->surfaceNumber){
                //std::cout << "checking edges" << std::endl;

                for(const auto& edge : facesList[j]->accessBSImpl()->edgesList){
                    if(edge->accessEImpl()->convexity == 0
                            && facesList[i]->accessBSImpl()->edgeOnSurface(*edge)){
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
McCAD::Decomposition::DecomposeSolid::Impl::selectSplitSurface(
        Geometry::Solid::Impl& solidImpl){
    // mergeSplitSurfaces(splitFacesList) // see no need for it as the formed lists; planesList, etc. are already merged. splitFacesList is a subset of facesList.
    SplitSurfaces::Impl::generateSplitFacesList(
                solidImpl.splitFacesList,
                solidImpl.selectedSplitFacesList);

    return !solidImpl.selectedSplitFacesList.empty();
}
