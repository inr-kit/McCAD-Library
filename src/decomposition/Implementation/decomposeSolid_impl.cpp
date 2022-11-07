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
// OCC
#include <STEPControl_Writer.hxx>

McCAD::Decomposition::DecomposeSolid::Impl::Impl(const IO::InputConfig& inputConfig)
    : recurrenceDepth{0}, inputConfig{inputConfig}{
}

McCAD::Decomposition::DecomposeSolid::Impl::Impl(const IO::InputConfig& inputConfig,
                                                 const int& recurrenceDepth)
    : recurrenceDepth{recurrenceDepth}, inputConfig{inputConfig}{
}

McCAD::Decomposition::DecomposeSolid::Impl::~Impl(){
}

/** ********************************************************************
* @brief    Operator that performs decomposition on a planar solid.
* @param    solidObj is a McCAD planar solid object.
* @return   true if decomposition succeeded.
* @date     31/12/2020
* @modified 25/08/2022
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::DecomposeSolid::Impl::operator()(
        std::shared_ptr<Geometry::PLSolid>& solidObj){
    // Increment the recurrence depth by 1.
    ++recurrenceDepth;
    if(recurrenceDepth > inputConfig.recurrenceDepth){
        return false;
    }
    auto solidImpl = solidObj->accessSImpl();
    // Judge which surfaces are decompose surfaces from the generated list.
    solidObj->accessPSImpl()->judgeDecomposeSurfaces(solidImpl,
                                                     inputConfig.precision,
                                                     inputConfig.distanceTolerance);
    // Check if any of the boundary surfaces does split the solid. This is judged
    // based on the numberCollidingSurfaces
    if(!throughNoBoundarySurfaces(solidImpl->splitFacesList)){
        solidObj->accessPSImpl()->judgeThroughConcaveEdges(solidImpl,
                                                           inputConfig.distanceTolerance);
    }
     return perform(*solidImpl);
}

/** ********************************************************************
* @brief    Operator that performs decomposition on a cylindrical solid.
* @param    solidObj is a McCAD cylindrical solid object.
* @return   true if decomposition succeeded.
* @date     31/12/2020
* @modified 25/08/2022
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::DecomposeSolid::Impl::operator()(
        std::shared_ptr<Geometry::CYLSolid>& solidObj){
    // Increment the recurrence depth by 1.
    ++recurrenceDepth;
    if(recurrenceDepth > inputConfig.recurrenceDepth){
        return false;
    }
    auto solidImpl = solidObj->accessSImpl();
    // Judge which surfaces are decompose surfaces from the generated list.
    solidObj->accessCSImpl()->judgeDecomposeSurfaces(solidImpl,
                                                     inputConfig.precision,
                                                     inputConfig.distanceTolerance);
    // Check if any of the boundary surfaces does split the solid. This is judged
    // based on the numberCollidingSurfaces
    if(!throughNoBoundarySurfaces(solidImpl->splitFacesList)){
        if (!planeSplitOnlyPlane(solidImpl->splitFacesList)){
            AssistSurfaceGenerator{inputConfig}(*solidObj);
            solidObj->accessCSImpl()->judgeAssistDecomposeSurfaces(solidImpl,
                                                                   inputConfig.precision,
                                                                   inputConfig.distanceTolerance);
         }
        solidObj->accessCSImpl()->judgeThroughConcaveEdges(solidImpl, 
                                                           inputConfig.distanceTolerance);
    }
    return perform(*solidImpl);
}

/** ********************************************************************
* @brief    Operator that performs decomposition on a toroidal solid.
* @param    solidObj is a McCAD toroidal solid object.
* @return   true if decomposition succeeded.
* @date     31/12/2020
* @modified 25/08/2022
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::DecomposeSolid::Impl::operator()(
        std::shared_ptr<Geometry::TORSolid>& solidObj){
    // Increment the recurrence depth by 1.
    ++recurrenceDepth;
    if(recurrenceDepth > inputConfig.recurrenceDepth){
        return false;
    }
    auto solidImpl = solidObj->accessSImpl();
    // Judge which surfaces are decompose surfaces from the generated list.
    solidObj->accessTSImpl()->judgeDecomposeSurfaces(solidImpl,
                                                     inputConfig.precision,
                                                     inputConfig.distanceTolerance);
    if(!throughNoBoundarySurfaces(solidImpl->splitFacesList)){
        // If the toroidal solid has split surface then use it.
        // If not, then judge if part torus or full torus and generate assisting surfces
        if (!planeSplitOnlyPlane(solidImpl->splitFacesList)){
            AssistSurfaceGenerator{inputConfig}(*solidObj);
            solidObj->accessTSImpl()->judgeAssistDecomposeSurfaces(solidImpl,
                                                                   inputConfig.precision,
                                                                   inputConfig.distanceTolerance);
        }
        solidObj->accessTSImpl()->judgeThroughConcaveEdges(solidImpl, 
                                                           inputConfig.distanceTolerance);
    }
    return perform(*solidImpl);
}

/** ********************************************************************
* @brief    Operator that performs decomposition on solids that contain a 
            mix of all the acceptable surface types; planar, cylindrical, 
            toroidal, and conical.
* @param    solidObj is a McCAD mixed solid object.
* @return   true if decomposition succeeded.
* @date     31/12/2020
* @modified 25/08/2022
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::DecomposeSolid::Impl::operator()(
        std::shared_ptr<Geometry::MXDSolid>& solidObj){
    //**** Review support for conical surfaces! ****
    // Increment the recurrence depth by 1.
    ++recurrenceDepth;
    if(recurrenceDepth > inputConfig.recurrenceDepth){
        return false;
    }
    auto solidImpl = solidObj->accessSImpl();
    // Judge which surfaces are decompose surfaces from the generated list.
    solidObj->accessXSImpl()->judgeDecomposeSurfaces(solidImpl,
                                                     inputConfig.precision,
                                                     inputConfig.distanceTolerance);
    // Check if any of the boundary surfaces does split the solid. This is judged
    // based on the numberCollidingSurfaces.
    if(!throughNoBoundarySurfaces(solidImpl->splitFacesList)){
        if (!planeSplitOnlyPlane(solidImpl->splitFacesList)){
            AssistSurfaceGenerator{inputConfig}(*solidObj);
            solidObj->accessXSImpl()->judgeAssistDecomposeSurfaces(solidImpl,
                                                                   inputConfig.precision,
                                                                   inputConfig.distanceTolerance);
        }
        solidObj->accessXSImpl()->judgeThroughConcaveEdges(solidImpl, 
                                                           inputConfig.distanceTolerance);
    }
    return perform(*solidImpl);
}

/** ********************************************************************
* @brief    Operator that performs decomposition on a conical solid.
* @param    solidObj is a McCAD conical solid object.
* @return   true if decomposition succeeded.
* @date     31/12/2020
* @modified 25/08/2022
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::DecomposeSolid::Impl::operator()(
    std::shared_ptr<Geometry::CONSolid>& solidObj) {
    // Increment the recurrence depth by 1.
    ++recurrenceDepth;
    if (recurrenceDepth > inputConfig.recurrenceDepth) {
        return false;
    }
    auto solidImpl = solidObj->accessSImpl();
    // Judge which surfaces are decompose surfaces from the generated list.
    solidObj->accessCONSImpl()->judgeDecomposeSurfaces(solidImpl,
                                                       inputConfig.precision,
                                                       inputConfig.distanceTolerance);
    // Check if any of the boundary surfaces does split the solid. This is judged
    // based on the numberCollidingSurfaces
    if (!throughNoBoundarySurfaces(solidImpl->splitFacesList)) {
        if (!planeSplitOnlyPlane(solidImpl->splitFacesList)) {
            AssistSurfaceGenerator{ inputConfig }(*solidObj);
            solidObj->accessCONSImpl()->judgeAssistDecomposeSurfaces(solidImpl,
                                                                     inputConfig.precision,
                                                                     inputConfig.distanceTolerance);
        }
        solidObj->accessCONSImpl()->judgeThroughConcaveEdges(solidImpl, 
                                                             inputConfig.distanceTolerance);
    }
    return perform(*solidImpl);
}

/** ********************************************************************
* @brief    A function that performs decomposition on a solid.
* @param    solidImpl is a McCAD solid object.
* @return   true if decomposition succeeded.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::DecomposeSolid::Impl::perform(Geometry::Solid::Impl& solidImpl){
    if(solidImpl.splitSurface){
        if (!selectSplitSurface(solidImpl)){
            return false;
        }
        // Try more than one splitting surface before prompting a fail decomposition.
        bool decompositionSuccess{false};
        for(int surfIndex = 0; surfIndex < solidImpl.selectedSplitFacesList.size();
            ++surfIndex){
            if(SolidDecomposer{inputConfig.debugLevel}(solidImpl.solid, solidImpl.obb,
                                 *solidImpl.selectedSplitFacesList[surfIndex],
                                 *solidImpl.splitSolidList)){
                decompositionSuccess = true;
                break;
            }
        }
        if(!decompositionSuccess) return false;
        // Loop over the resulting subsolids and split each one of them recursively.
        for (int i = 1; i <= solidImpl.splitSolidList->Length(); ++i){
            auto subSolid = Preprocessor{inputConfig}.perform(
                        solidImpl.splitSolidList->Value(i));
            if (std::holds_alternative<std::monostate>(subSolid)){
                solidImpl.rejectedsubSolidsList->Append(solidImpl.splitSolidList->Value(i));
                continue;
            }
            // Using switch for now. Should be separated in a separate class an called
            // for each specific type of solid object.
            switch (int(subSolid.index())){
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
            } case solidType.mixed:{
                auto& subSolidImpl = *std::get<solidType.mixed>(subSolid)->accessSImpl();
                if (DecomposeSolid::Impl{inputConfig, recurrenceDepth}(std::get<solidType.mixed>(subSolid))){
                    extractSolids(solidImpl, subSolidImpl, i);
                } else solidImpl.rejectedsubSolidsList->Append(subSolidImpl.solid);
                break;
            } case solidType.conical: {
                auto& subSolidImpl = *std::get<solidType.conical>(subSolid)->accessSImpl();
                if (DecomposeSolid::Impl{ inputConfig, recurrenceDepth }(std::get<solidType.conical>(subSolid))) {
                    extractSolids(solidImpl, subSolidImpl, i);
                }
                else solidImpl.rejectedsubSolidsList->Append(subSolidImpl.solid);
                break;
            } default:
                solidImpl.rejectedsubSolidsList->Append(solidImpl.splitSolidList->Value(i));
            }
        }
    } else{
        if(solidImpl.rejectSolid) return false;
        else solidImpl.splitSolidList->Append(solidImpl.solid);
    }
    return true;
}

/** ********************************************************************
* @brief    A function that selectes a split surface from a list of candidate split surfaces.
* @param    solidImpl is a McCAD solid objects.
* @return   true if a surface is selected.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::DecomposeSolid::Impl::selectSplitSurface(
        Geometry::Solid::Impl& solidImpl){
    SplitSurfacesSelector{}.generateSplitFacesList(solidImpl.splitFacesList,
                                                   solidImpl.selectedSplitFacesList);
    return !solidImpl.selectedSplitFacesList.empty();
}

/** ********************************************************************
* @brief    A function that judges if a split surface goes through a boundary surface.
* @param    facesList is a McCAD list of suface objects.
* @return   true if split faces go through no boundary faces.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::DecomposeSolid::Impl::throughNoBoundarySurfaces(
        const std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList){
    return std::any_of(facesList.cbegin(), facesList.cend(),
                       [](const std::shared_ptr<Geometry::BoundSurface>& face){
        return face->accessSImpl()->numberCollidingSurfaces == 0;
    });
}

/** ********************************************************************
* @brief    A function that judges if a split surface goes through only a planar surface.
* @param    facesList is a McCAD list of suface objects.
* @return  true if planes only split planes.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
bool
McCAD::Decomposition::DecomposeSolid::Impl::planeSplitOnlyPlane(
        const std::vector<std::shared_ptr<Geometry::BoundSurface>>& facesList){
    return std::any_of(facesList.cbegin(), facesList.cend(),
                       [](const std::shared_ptr<Geometry::BoundSurface>& face){
        return face->getSurfaceType() == Tools::toTypeName(GeomAbs_Plane)
                && face->accessSImpl()->numberCollidingCurvedSurfaces == 0;
    });
}

/** ********************************************************************
* @brief    A function that extracts the subsolids after decomposition.
* @param    solidImpl is a McCAD solid object.
* @param    subSolidImpl is a McCAD solid object.
* @param    i is the order of the parent solid in the list of input solids.
* @date     31/12/2020
* @modified
* @author   Moataz Harb
* **********************************************************************/
void
McCAD::Decomposition::DecomposeSolid::Impl::extractSolids(
        Geometry::Solid::Impl& solidImpl,
        const Geometry::Solid::Impl& subSolidImpl,
        int& i){
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
