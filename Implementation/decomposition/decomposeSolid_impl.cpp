// McCAD
#include "decomposeSolid_impl.hpp"
#include "SurfaceUtilities.hpp"

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
    //std::cout << "     - Recurrence Depth: " << recurrenceDepth << std::endl;
    if(recurrenceDepth >= 20){
        return Standard_False;
    }
    auto solidImpl = solidObj->accessSImpl();
    // Judge which surfaces are decompose surfaces from the generated list.
    solidObj->accessPSImpl()->judgeDecomposeSurfaces(solidImpl);
    if(!throughNoBoundarySurfaces(solidImpl->splitFacesList)){
        //std::cout << "no throughNoBoundarySurfaces" << std::endl;
        solidObj->accessPSImpl()->judgeThroughConcaveEdges(solidImpl);
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
    //std::cout << "     - Recurrence Depth: " << recurrenceDepth << std::endl;
    if(recurrenceDepth >= 20){
        return Standard_False;
    }
    auto solidImpl = solidObj->accessSImpl();
    // Judge which surfaces are decompose surfaces from the generated list.
    solidObj->accessCSImpl()->judgeDecomposeSurfaces(solidImpl);
    if(!throughNoBoundarySurfaces(solidImpl->splitFacesList)){
        solidObj->accessCSImpl()->judgeThroughConcaveEdges(solidImpl);
         if (!planeSplitOnlyPlane(solidImpl->splitFacesList)){
             //generateAssistingSurfaces();
             //judgeAssistingDecomposeSurfaces();
             //judgeThroughConcaveEdges(assistingFacesList);
         }
    }
    return perform(*solidImpl);
}

void
McCAD::Decomposition::DecomposeSolid::Impl::splitSubsolid(
        Geometry::Solid::Impl& solidImpl,
        Standard_Integer& index){
    auto subSolid = Preprocessor{}.perform(solidImpl.splitSolidList->Value(index));
    if (std::holds_alternative<std::monostate>(subSolid)){
        solidImpl.rejectedsubSolidsList->Append(solidImpl.splitSolidList->Value(index));
        return;
    }
    // Using switch for now. Should be separated in a separate class an called
    // for each specific type of solid object.
    switch (Standard_Integer(subSolid.index())){
    case solidType.planarSolid:{
        auto& subSolidImpl = *std::get<solidType.planarSolid>(subSolid)->accessSImpl();
        // Mesh deflection is calculated for every solid in DecomposeSolid.
        if (DecomposeSolid::Impl{recurrenceDepth}(std::get<solidType.planarSolid>(subSolid))){
            extractSolids(solidImpl, subSolidImpl, index);
        } else{
            //return Standard_False;
            solidImpl.rejectedsubSolidsList->Append(subSolidImpl.solid);
        }
        break;
    } case solidType.cylindricalSolid:{
        auto& subSolidImpl = *std::get<solidType.cylindricalSolid>(subSolid)->accessSImpl();
        if (DecomposeSolid::Impl{recurrenceDepth}(std::get<solidType.cylindricalSolid>(subSolid))){
            extractSolids(solidImpl, subSolidImpl, index);
        } else{
            solidImpl.rejectedsubSolidsList->Append(subSolidImpl.solid);
        }
        break;
    } default:
        solidImpl.rejectedsubSolidsList->Append(solidImpl.splitSolidList->Value(index));
    }
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
            std::cout << "SplitSolid return false" << std::endl;
            return Standard_False;
        }
        // Loop over the resulting subsolids and split each one of them recursively.
        for (Standard_Integer i = 1; i <= solidImpl.splitSolidList->Length(); ++i){
            //std::cout << "   - Decomposing subsolid # " << recurrenceDepth << "/"
            //          << solidImpl.splitSolidList->Length() << "/" << i << std::endl;
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

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::selectSplitSurface(
        Geometry::Solid::Impl& solidImpl){
    SplitSurfaces::Impl::generateSplitFacesList(solidImpl.splitFacesList,
                                                solidImpl.selectedSplitFacesList);
    return !solidImpl.selectedSplitFacesList.empty();
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::throughNoBoundarySurfaces(
        const std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList){
    return std::any_of(facesList.cbegin(), facesList.cend(),
                       [](const std::shared_ptr<Geometry::BoundSurface>& face){
        return face->accessSImpl()->numberCollidingSurfaces == 0;
    });
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::planeSplitOnlyPlane(
        const std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList){
    return std::any_of(facesList.cbegin(), facesList.cend(),
                       [](const std::shared_ptr<Geometry::BoundSurface>& face){
        return face->getSurfaceType() == Tools::toTypeName(GeomAbs_Plane)
                && face->accessSImpl()->numberCollidingCurvedSurfaces == 0;
    });
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
