// McCAD
#include "decomposition_impl.hpp"
#include "TaskQueue.hpp"
#include "preprocessor.hpp"
#include "planarSolid_impl.hpp"
#include "cylSolid_impl.hpp"
#include "torSolid_impl.hpp"
#include "torusConvertor.hpp"

McCAD::Decomposition::Decompose::Impl::Impl(const General::InputData& inputData,
                                            const IO::InputConfig& inputConfig)
  : inputConfig{inputConfig}{
    // Get input solids list from the Input Data object.
    inputShapesMap = inputData.accessImpl()->inputShapesMap;
    if (!inputShapesMap.size() > 0)
        throw std::runtime_error("Error reading input STEP file!");
    std::cout << " > Decomposing " << inputShapesMap.size() <<
                 " shap(s) from the input STEP file" << std::endl;
    perform();
}

McCAD::Decomposition::Decompose::Impl::~Impl(){
}

void
McCAD::Decomposition::Decompose::Impl::perform(
        const McCAD::Decomposition::Decompose::Impl::shapeTuple& inputShape){
    std::unique_ptr<Geometry::Impl::Compound> compoundObj =
            std::make_unique<Geometry::Impl::Compound>(
            std::get<0>(inputShape), std::get<1>(inputShape));
    Preprocessor{compoundObj};
    for(auto& plSolid : compoundObj->planarSolidsList){
        std::cout << "   - Decomposing planar solid" << std::endl;
        if (DecomposeSolid{inputConfig}.accessDSImpl()->operator()(plSolid)){
            extractSolids(compoundObj, plSolid);
        } else compoundObj->rejectedInputShapesList->Append(
                    plSolid->accessSImpl()->solid);
    }
    for(auto& cylSolid : compoundObj->cylSolidsList){
        std::cout << "   - Decomposing cylindrical solid" << std::endl;
        if (DecomposeSolid{inputConfig}.accessDSImpl()->operator()(cylSolid)){
            extractSolids(compoundObj, cylSolid);
        } else compoundObj->rejectedInputShapesList->Append(
                    cylSolid->accessSImpl()->solid);
    }
    for(auto& torSolid : compoundObj->torSolidsList){
        std::cout << "   - Decomposing toroidal solid" << std::endl;
        if (DecomposeSolid{inputConfig}.accessDSImpl()->operator()(torSolid)){
            // Convert tori to cylinders.
            TorusConvertor{}(torSolid);
            extractSolids(compoundObj, torSolid);
        } else compoundObj->rejectedInputShapesList->Append(
                    torSolid->accessSImpl()->solid);
    }
    compoundList.push_back(std::move(compoundObj));
}

void
McCAD::Decomposition::Decompose::Impl::perform(){
    TaskQueue<Policy::Parallel> taskQueue;
    for(const auto& member : inputShapesMap){
        taskQueue.submit([this, &member](){
            perform(member);
        });
    }
    taskQueue.complete();
    // Extract result solids into maps.
    for(const auto& compound : compoundList){
        // Add successful subolids
        successDecomposition.push_back(std::make_tuple(compound->compoundName,
                                                       *compound->subSolidsList));
        // Add rejected input solids and failed subolids
        if(compound->rejectedInputShapesList->Length() > 0){
            TopTools_HSequenceOfShape rejected{*compound->rejectedInputShapesList};
            if(compound->rejectedsubSolidsList->Length() > 0)
                rejected.Append(*compound->rejectedsubSolidsList);
            rejectDecomposition.push_back(std::make_tuple(compound->compoundName,
                                                      rejected));
        }
    }
    std::cout << " > Results:" << std::endl;
    std::cout << "   - Decomposition failed for " << rejectDecomposition.size() <<
                 " input shape(s)." << std::endl;
    std::cout << "   - Decomposition succeeded for " << successDecomposition.size() <<
                 " input shape(s)." << std::endl;
}

void
McCAD::Decomposition::Decompose::Impl::extractSolids(
        const std::unique_ptr<Geometry::Impl::Compound>& compound,
        const std::shared_ptr<Geometry::Solid>& solid){
    for(const auto& resultSolid : *solid->accessSImpl()->splitSolidList){
        compound->subSolidsList->Append(resultSolid);
    }
    if (solid->accessSImpl()->rejectedsubSolidsList->Length() > 0){
        for(const auto& rejectedSubSolid : *solid->accessSImpl()->rejectedsubSolidsList){
            compound->rejectedsubSolidsList->Append(rejectedSubSolid);
        }
    }
}
