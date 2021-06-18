// C++
#include <variant>
#include <tuple>
#include <algorithm>
// McCAD
#include "decomposeSolid_impl.hpp"
#include "splitSurfacesSelector.hpp"
#include "solidDecomposer.hpp"
#include "AssistSurfaceGenerator.hpp"
#include "preprocessor.hpp"
#include "SurfaceUtilities.hpp"

McCAD::Decomposition::DecomposeSolid::Impl::Impl(const IO::InputConfig& inputConfig)
    : recurrenceDepth{0}, inputConfig{inputConfig}{
}

McCAD::Decomposition::DecomposeSolid::Impl::Impl(const IO::InputConfig& inputConfig,
                                                 const Standard_Integer& recurrenceDepth)
    : recurrenceDepth{recurrenceDepth}, inputConfig{inputConfig}{
}

McCAD::Decomposition::DecomposeSolid::Impl::~Impl(){
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::operator()(
        std::shared_ptr<Geometry::PLSolid>& solidObj){
    // Increment the recurrence depth by 1.
    ++recurrenceDepth;
    if(recurrenceDepth > inputConfig.recurrenceDepth){
        return Standard_False;
    }
    auto solidImpl = solidObj->accessSImpl();
    // Judge which surfaces are decompose surfaces from the generated list.
    solidObj->accessPSImpl()->judgeDecomposeSurfaces(solidImpl);
    // Check if any of the boundary surfaces does split the solid. This is judged
    // based on the numberCollidingSurfaces
    if(!throughNoBoundarySurfaces(solidImpl->splitFacesList)){
        solidObj->accessPSImpl()->judgeThroughConcaveEdges(solidImpl);
    }
     return perform(*solidImpl);
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::operator()(
        std::shared_ptr<Geometry::CYLSolid>& solidObj){
    // Increment the recurrence depth by 1.
    ++recurrenceDepth;
    if(recurrenceDepth > inputConfig.recurrenceDepth){
        return Standard_False;
    }
    auto solidImpl = solidObj->accessSImpl();
    // Judge which surfaces are decompose surfaces from the generated list.
    solidObj->accessCSImpl()->judgeDecomposeSurfaces(solidImpl);
    // Check if any of the boundary surfaces does split the solid. This is judged
    // based on the numberCollidingSurfaces
    if(!throughNoBoundarySurfaces(solidImpl->splitFacesList)){
        if (!planeSplitOnlyPlane(solidImpl->splitFacesList)){
            AssistSurfaceGenerator{inputConfig}(*solidObj);
            solidObj->accessCSImpl()->judgeAssistDecomposeSurfaces(solidImpl);
         }
        solidObj->accessCSImpl()->judgeThroughConcaveEdges(solidImpl);
    }
    return perform(*solidImpl);
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::operator()(
        std::shared_ptr<Geometry::TORSolid>& solidObj){
    // Increment the recurrence depth by 1.
    ++recurrenceDepth;
    if(recurrenceDepth >= inputConfig.recurrenceDepth){
        return Standard_False;
    }
    auto solidImpl = solidObj->accessSImpl();
    // Judge which surfaces are decompose surfaces from the generated list.
    solidObj->accessTSImpl()->judgeDecomposeSurfaces(solidImpl);
    if(!throughNoBoundarySurfaces(solidImpl->splitFacesList)){
        solidObj->accessTSImpl()->judgeThroughConcaveEdges(solidImpl);
        // If the toroidal solid has split surface then use it.
        // If not, then judge if part torus or full torus and generate assisting surfces
        AssistSurfaceGenerator{inputConfig}(*solidObj);
    }
    return perform(*solidImpl);
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::perform(Geometry::Solid::Impl& solidImpl){
    if(solidImpl.splitSurface){
        if (!selectSplitSurface(solidImpl)){
            return Standard_False;
        }
        // Try more than one splitting surface before prompting a fail decomposition.
        Standard_Boolean decompositionSuccess{Standard_False};
        for(Standard_Integer surfIndex = 0; surfIndex < solidImpl.selectedSplitFacesList.size();
            ++surfIndex){
            if(SolidDecomposer{}(solidImpl.solid, solidImpl.obb,
                                 *solidImpl.selectedSplitFacesList[surfIndex],
                                 *solidImpl.splitSolidList)){
                decompositionSuccess = Standard_True;
                break;
            }
        }
        if(!decompositionSuccess) return Standard_False;
        // Loop over the resulting subsolids and split each one of them recursively.
        for (Standard_Integer i = 1; i <= solidImpl.splitSolidList->Length(); ++i){
            auto subSolid = Preprocessor{inputConfig}.perform(
                        solidImpl.splitSolidList->Value(i));
            if (std::holds_alternative<std::monostate>(subSolid)){
                solidImpl.rejectedsubSolidsList->Append(solidImpl.splitSolidList->Value(i));
                continue;
            }
            // Using switch for now. Should be separated in a separate class an called
            // for each specific type of solid object.
            switch (Standard_Integer(subSolid.index())){
            case solidType.planar:{
                auto& subSolidImpl = *std::get<solidType.planar>(subSolid)->accessSImpl();
                // Mesh deflection is calculated for every solid in DecomposeSolid.
                if (DecomposeSolid::Impl{inputConfig, recurrenceDepth}(std::get<solidType.planar>(subSolid))){
                    extractSolids(solidImpl, subSolidImpl, i);
                } else solidImpl.rejectedsubSolidsList->Append(subSolidImpl.solid);
                break;
            } case solidType.cylindrical:{
                auto& subSolidImpl = *std::get<solidType.cylindrical>(subSolid)->accessSImpl();
                if (DecomposeSolid::Impl{inputConfig, recurrenceDepth}(std::get<solidType.cylindrical>(subSolid))){
                    extractSolids(solidImpl, subSolidImpl, i);
                } else solidImpl.rejectedsubSolidsList->Append(subSolidImpl.solid);
                break;
            } case solidType.toroidal:{
                auto& subSolidImpl = *std::get<solidType.toroidal>(subSolid)->accessSImpl();
                if (DecomposeSolid::Impl{inputConfig, recurrenceDepth}(std::get<solidType.toroidal>(subSolid))){
                    extractSolids(solidImpl, subSolidImpl, i);
                } else solidImpl.rejectedsubSolidsList->Append(subSolidImpl.solid);
                break;
            } default:
                solidImpl.rejectedsubSolidsList->Append(solidImpl.splitSolidList->Value(i));
            }
        }
    } else{
        solidImpl.splitSolidList->Append(solidImpl.solid);
    }
    return Standard_True;
}

Standard_Boolean
McCAD::Decomposition::DecomposeSolid::Impl::selectSplitSurface(
        Geometry::Solid::Impl& solidImpl){
    SplitSurfacesSelector{}.generateSplitFacesList(solidImpl.splitFacesList,
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
    if (subSolidImpl.splitSolidList->Length() >= 1){
        for(const auto& resultsubSolid : *subSolidImpl.splitSolidList){
            solidImpl.splitSolidList->InsertAfter(i, resultsubSolid);
        }
        solidImpl.splitSolidList->Remove(i);
        i += subSolidImpl.splitSolidList->Length() - 1;
    }
    // Add rejected subSolids
    solidImpl.rejectedsubSolidsList->Append(*subSolidImpl.rejectedsubSolidsList);
}
