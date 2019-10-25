// McCAD
#include "decomposeSolid_impl.hpp"

McCAD::Decomposition::DecomposeSolid::Impl::Impl()
    : recurrenceDepth{0}{
}

McCAD::Decomposition::DecomposeSolid::Impl::Impl(Standard_Integer recurrenceDepth)
    : recurrenceDepth{recurrenceDepth}{
}

McCAD::Decomposition::DecomposeSolid::Impl::~Impl(){
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::operator()(Geometry::Solid::Impl& solidImpl){
    return perform(solidImpl);
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::perform(Geometry::Solid::Impl& solidImpl){
    // The function will be called recursively on a solid.
    // A condition has to be set for termination (Now only 20 recursion levels).
    // Increment the recurrence depth by 1.
    ++recurrenceDepth;
    std::cout << "     - Recurrence Depth: " << recurrenceDepth << std::endl;
    // Judge which surfaces are decompose surfaces from the generated list.
    judgeDecomposeSurfaces(solidImpl);
    //judgeThroughConcaveEdges(splitFacesList);
    if(!SplitSurfaces::Impl::throughNoBoundarySurfaces(solidImpl.splitFacesList)){
        //std::cout << "throughNoBoundarySurfaces false" << std::endl;
        judgeThroughConcaveEdges(solidImpl.splitFacesList);
     /*
     if (!splitSurfaces.accessSSImpl()->planeSplitOnlyPlane(splitFacesList)){
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
        // std::cout << "selected surface concave edges: " <<
        //             selectedSplitFacesList[0]->accessSImpl()->throughConcaveEdges << std::endl;
        if(!(SplitSolid::Impl{}(solidImpl.solid, solidImpl.obb,
                                *solidImpl.selectedSplitFacesList[0],
                                *solidImpl.splitSolidList))){
            //std::cout << "return of splitsolid" << std::endl;
            return Standard_False;
        }
        // Loop over the resulting subsolids and split each one of them recursively.
        //std::cout << "splitting subsolids" << std::endl;
        for (Standard_Integer i = 1; i <= solidImpl.splitSolidList->Length(); ++i){
            std::cout << "   - Decomposing subsolid # " << recurrenceDepth << "/"
                      << solidImpl.splitSolidList->Length() << "/" << i << std::endl;
            //std::cout << splitSolidList->Length() << std::endl;
            auto subSolid = Preprocessor{}.perform(solidImpl.splitSolidList->Value(i));
            if (std::holds_alternative<std::monostate>(subSolid)){
                solidImpl.rejectedsubSolidsList->Append(solidImpl.splitSolidList->Value(i));
                continue;
            }
            // Using switch for now. Should be separated in a separate class an called
            // for each specific type of solid object.
            switch (Standard_Integer(subSolid.index())){
            case solidType.planarSolid:{
                auto& subSolidImpl = *std::get<solidType.planarSolid>(subSolid)->accessSImpl();
                // Mesh deflection is calculated for every solid in DecomposeSolid.
                if (DecomposeSolid::Impl{recurrenceDepth}(subSolidImpl)){
                    extractSolids(solidImpl, subSolidImpl, i);
                    /*
                    if (subSolidImpl.splitSolidList->Length() >= 2){
                        //splitSolidList->Remove(i);
                        for(const auto& el : *subSolidImpl.splitSolidList){
                            solidImpl.splitSolidList->InsertAfter(i, el);
                        }
                        solidImpl.splitSolidList->Remove(i);
                        i += subSolidImpl.splitSolidList->Length() - 1;
                    }
                    // Add rejected subSolids
                    solidImpl.rejectedsubSolidsList->Append(
                                *subSolidImpl.rejectedsubSolidsList);
                    //return Standard_True;
                    */
                } else{
                    //return Standard_False;
                    solidImpl.rejectedsubSolidsList->Append(subSolidImpl.solid);
                }
                break;}
            case solidType.cylindricalSolid:{
                solidImpl.rejectedsubSolidsList->Append(solidImpl.splitSolidList->Value(i));
                break;}
            case solidType.sphericalSolid:{
                solidImpl.rejectedsubSolidsList->Append(solidImpl.splitSolidList->Value(i));
                break;}
            default:
                solidImpl.rejectedsubSolidsList->Append(solidImpl.splitSolidList->Value(i));
            }
            //return Standard_True;
        }
    } else{
        //std::cout	<< "Solid has no split surfaces" << std::endl;
        solidImpl.splitSolidList->Append(solidImpl.solid);
    }
    return Standard_True;
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
    //std::cout << "facesList.size(): " << facesList.size() << std::endl;
    for (Standard_Integer i = 0; i <= facesList.size() - 1; ++i){
        auto iFace = facesList[i]->accessSImpl();
        //std::cout << "judge: " << i << std::endl;;
        Standard_Integer positiveFaces = 0;
        Standard_Integer negativeFaces = 0;
        Standard_Integer numberCollidingSurfaces = 0;
        Standard_Integer numberCollidingCurvedSurfaces = 0;
        for (Standard_Integer j = 0; j <= facesList.size() - 1; ++j){
            auto jFace = facesList[j]->accessSImpl();
            //std::cout << "judge:    " << j << std::endl;
            if (i != j && iFace->surfaceNumber != jFace->surfaceNumber){
                Standard_Integer side = 0;
                if (facesList[i]->accessBSImpl()->faceCollision(*facesList[j], side)){
                    ++numberCollidingSurfaces;
                    //std::cout << "facecollision True" << std::endl;
                    iFace->splitSurface = Standard_True;
                    if (facesList[j]->getSurfaceType() != "Plane"){
                        ++numberCollidingCurvedSurfaces;
                    }
                } else{
                    //std::cout << "facecollision False" << std::endl;
                    //std::cout << "side" << side << std::endl;
                    if (side == 1){
                        ++positiveFaces;
                    } else if (side == -1){
                        ++negativeFaces;
                    }
                }
            }
        }
        if (positiveFaces > 0 && negativeFaces > 0){
            //std::cout << "splitsutface True, pos & neg" << std::endl;
            iFace->splitSurface = Standard_True;
        }
        if (facesList[i]->accessSImpl()->splitSurface){
            //std::cout << "set collidingsurfaces" << std::endl;
            iFace->numberCollidingSurfaces = numberCollidingSurfaces;
            iFace->numberCollidingCurvedSurfaces = numberCollidingCurvedSurfaces;
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
    if (facesList.size() == 0){
        //std::cout << "return" << std::endl;
        return;
    }
    for (Standard_Integer i = 0; i <= facesList.size() - 1; ++i){
        // Don't update throughConcaveEdges if it already has a value.
        // Only update surfaces that result from fusing others.
        if (facesList[i]->accessSImpl()->throughConcaveEdges != 0) continue;
        Standard_Integer throughConcaveEdges = 0;
        for (Standard_Integer j = 0; j <= facesList.size() - 1; ++j){
            if (i != j && facesList[i]->accessSImpl()->surfaceNumber !=
                    facesList[j]->accessSImpl()->surfaceNumber){
                //std::cout << "checking edges" << std::endl;
                for (Standard_Integer k = 0; k <= facesList[j]->accessBSImpl()->edgesList.size() - 1; ++k){
                    if (facesList[j]->accessBSImpl()->edgesList[k]->accessEImpl()->convexity
                            == 0 && facesList[i]->accessBSImpl()->edgeOnSurface(
                                *(facesList[j]->accessBSImpl()->edgesList[k]))){
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
    SplitSurfaces::Impl::generateSplitFacesList(solidImpl.splitFacesList,
                                                solidImpl.selectedSplitFacesList);
    return !solidImpl.selectedSplitFacesList.empty();
}

void
McCAD::Decomposition::DecomposeSolid::Impl::extractSolids(
        Geometry::Solid::Impl& solidImpl,
        const Geometry::Solid::Impl& subSolidImpl,
        Standard_Integer& i){
    if (subSolidImpl.splitSolidList->Length() >= 2){
        for(const auto& resultsubSolid : *subSolidImpl.splitSolidList){
            solidImpl.splitSolidList->InsertAfter(i, resultsubSolid);
        }
        solidImpl.splitSolidList->Remove(i);
        i += subSolidImpl.splitSolidList->Length() - 1;
    }
    // Add rejected subSolids
    solidImpl.rejectedsubSolidsList->Append(*subSolidImpl.rejectedsubSolidsList);
}
