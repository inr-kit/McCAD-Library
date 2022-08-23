// McCAD
#include "decomposition_impl.hpp"
#include "decomposeSolid_impl.hpp"
#include "ShapeView.hpp"
#include "TaskQueue.hpp"
#include "preprocessor.hpp"
#include "planarSolid_impl.hpp"
#include "cylSolid_impl.hpp"
#include "torSolid_impl.hpp"
#include "torusConvertor.hpp"

/** ********************************************************************
* @brief   The main constructor that executes McCAD decomposition. 
* @param   inputData is an object containing list(s) of solids from the input STEP file(s). 
* @param   inputConfig is an object containing the values of the configuration parameters. 
* @date    31/12/2020
* @author  Moataz Harb & Christian Wegmann
* **********************************************************************/
McCAD::Decomposition::Decompose::Impl::Impl(const General::InputData& inputData,
                                            const IO::InputConfig& inputConfig)
  : inputConfig{inputConfig}{
    // Get input solids map from the Input Data object.
    inputShapesMap = inputData.accessImpl()->inputShapesMap;
    if (!inputShapesMap.size() > 0)
        throw std::runtime_error("Error loading shape(s) from the input STEP file(s)!");
    std::cout << " > Decomposing " << inputShapesMap.size() <<
                 " shape(s) from the input STEP file(s)" << std::endl;
    perform();
}

McCAD::Decomposition::Decompose::Impl::~Impl(){
}

/** ********************************************************************
* @brief    The main function that executes McCAD decomposition on an input solid.
* @param    inputShape is a tuple containing the input shape and its name. 
* @param    compoundID is a unique ID, counter, to assing to the compound object to be created.
* @date     31/12/2022
* @modified 23/08/2022
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
void
McCAD::Decomposition::Decompose::Impl::perform(
        const McCAD::Decomposition::Decompose::Impl::shapeTuple& inputShape,
        const int& compoundID){
    std::shared_ptr<Geometry::Impl::Compound> compoundObj =
            std::make_unique<Geometry::Impl::Compound>(
            std::get<0>(inputShape), std::get<1>(inputShape));
    compoundObj->compoundID = compoundID;
    Preprocessor{inputConfig}(compoundObj);
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
            if(inputConfig.simplifyTori) TorusConvertor{inputConfig}(torSolid);
            extractSolids(compoundObj, torSolid);
        } else compoundObj->rejectedInputShapesList->Append(
                    torSolid->accessSImpl()->solid);
    }
    for(auto& mxdSolid : compoundObj->mixedSolidsList){
        std::cout << "   - Decomposing mixed solid" << std::endl;
        if (DecomposeSolid{inputConfig}.accessDSImpl()->operator()(mxdSolid)){
            // Convert tori to cylinders.
            if(inputConfig.simplifyTori) TorusConvertor{inputConfig}(mxdSolid);
            extractSolids(compoundObj, mxdSolid);
        } else compoundObj->rejectedInputShapesList->Append(
                    mxdSolid->accessSImpl()->solid);
    }
    for (auto& conSolid : compoundObj->conSolidsList) {
        std::cout << "   - Decomposing conical solid" << std::endl;
    }
    compoundList.push_back(std::move(compoundObj));
}

/** ********************************************************************
* @brief   The main function that performs decomposition in parallel on input solids.
* @date    31/12/2020
* @author  Moataz Harb & Christian Wegmann
* **********************************************************************/
void
McCAD::Decomposition::Decompose::Impl::perform(){
    TaskQueue<Policy::Parallel> taskQueue;
    int counter = 0;
    for(const auto& member : inputShapesMap){
        taskQueue.submit([this, counter, &member](){
            perform(member, counter);
        });
        ++counter;
    }
    taskQueue.complete();
    // Extract result solids into maps.
    for(const auto& compound : compoundList){
        // Add successful subolids
        if(compound->subSolidsList->Length() > 0){
            successDecomposition.push_back(std::make_tuple(compound->compoundName,
                                                           *compound->subSolidsList));
        }
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

/** ********************************************************************
* @brief   A function that extracts the resultant subsolids and adds it to the compound object.
* @param   solid is the solid object that was processed by decomposition.
* @param   compound is a compound object.
* @date    31/12/2020
* @author  Moataz Harb & Christian Wegmann
* **********************************************************************/
void
McCAD::Decomposition::Decompose::Impl::extractSolids(
        const std::shared_ptr<Geometry::Impl::Compound>& compound,
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
