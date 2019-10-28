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
McCAD::Decomposition::DecomposeSolid::Impl::operator()(
        std::shared_ptr<Geometry::PLSolid>& solidObj){
    // The operator will be called recursively on a solid.
    // A condition has to be set for termination (Now only 20 recursion levels).
    // Increment the recurrence depth by 1.
    ++recurrenceDepth;
    std::cout << "     - Recurrence Depth: " << recurrenceDepth << std::endl;
    if(recurrenceDepth >= 20){
        return Standard_False;
    }
    auto solidImpl = solidObj->accessSImpl();
    // Judge which surfaces are decompose surfaces from the generated list.
    solidObj->accessPSImpl()->judgeDecomposeSurfaces(solidImpl);
    if(!SplitSurfaces::Impl::throughNoBoundarySurfaces(solidImpl->splitFacesList)){
        judgeThroughConcaveEdges(solidImpl->splitFacesList);
    }
     return perform(*solidImpl);
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::operator()(
        std::shared_ptr<Geometry::CYLSolid>& solidObj){
    // The operator will be called recursively on a solid.
    // A condition has to be set for termination (Now only 20 recursion levels).
    // Increment the recurrence depth by 1.
    ++recurrenceDepth;
    std::cout << "     - Recurrence Depth: " << recurrenceDepth << std::endl;
    if(recurrenceDepth >= 20){
        return Standard_False;
    }
    auto solidImpl = solidObj->accessSImpl();
    // Judge which surfaces are decompose surfaces from the generated list.
    solidObj->accessCSImpl()->judgeDecomposeSurfaces(solidImpl);
    /*
    if(!SplitSurfaces::Impl::throughNoBoundarySurfaces(solidImpl->splitFacesList)){
        judgeThroughConcaveEdges(solidImpl->splitFacesList);
         if (!splitSurfaces.accessSSImpl()->planeSplitOnlyPlane(
                     solidImpl->splitFacesList)){
             //generateAssistingSurfaces();
             //judgeAssistingDecomposeSurfaces();
             //judgeThroughConcaveEdges(assistingFacesList);
         }
    }*/
    return Standard_False;
    //return perform(solidImpl);
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::perform(Geometry::Solid::Impl& solidImpl){
    if(solidImpl.splitSurface){
        //std::cout << "Solid has a split surface" << std::endl;
        if (!selectSplitSurface(solidImpl)){
            return Standard_False;
        }
        if(!(SplitSolid::Impl{}(solidImpl.solid, solidImpl.obb,
                                *solidImpl.selectedSplitFacesList[0],
                                *solidImpl.splitSolidList))){
            return Standard_False;
        }
        // Loop over the resulting subsolids and split each one of them recursively.
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
                if (DecomposeSolid::Impl{recurrenceDepth}(std::get<solidType.planarSolid>(subSolid))){
                    extractSolids(solidImpl, subSolidImpl, i);
                } else{
                    //return Standard_False;
                    solidImpl.rejectedsubSolidsList->Append(subSolidImpl.solid);
                }
                break;
            } case solidType.cylindricalSolid:{
                auto& subSolidImpl = *std::get<solidType.cylindricalSolid>(subSolid)->accessSImpl();
                if (DecomposeSolid::Impl{recurrenceDepth}(std::get<solidType.cylindricalSolid>(subSolid))){
                    extractSolids(solidImpl, subSolidImpl, i);
                } else{
                    solidImpl.rejectedsubSolidsList->Append(subSolidImpl.solid);
                }
                break;
            } default:
                solidImpl.rejectedsubSolidsList->Append(solidImpl.splitSolidList->Value(i));
            }
        }
    } else{
        //std::cout	<< "Solid has no split surfaces" << std::endl;
        solidImpl.splitSolidList->Append(solidImpl.solid);
    }
    return Standard_True;
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
