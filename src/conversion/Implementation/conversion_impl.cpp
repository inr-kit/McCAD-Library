// C++
#include <iostream>
// McCAD
#include "conversion_impl.hpp"
#include "stepreader.hpp"
#include "stepwriter.hpp"
#include "inputdata_impl.hpp"
#include "preprocessor.hpp"
#include "TaskQueue.hpp"
#include "voidCellManager.hpp"
#include "conversionWriter.hpp"

McCAD::Conversion::Convert::Impl::Impl(const IO::InputConfig& inputConfig) :
    inputConfig{inputConfig}{
    IO::STEPReader reader{inputConfig.conversionFileName};
    inputShapesMap = reader.getInputData().accessImpl()->inputShapesMap;
    if (!inputShapesMap.size() > 0)
        throw std::runtime_error("Error loading STEP file, " + inputConfig.conversionFileName);
    std::cout << "> Found " << inputShapesMap.size() <<
                 " shapes(s) in the input STEP file" << std::endl;
    getGeomData();
    if (rejectCondition){
        // Write rejected solids to a STEP file.
        General::InputData outputData;
        outputData.accessImpl()->outputShapesMap = rejectConversion;
        McCAD::IO::STEPWriter{"rejectedConversion", outputData};
        throw std::runtime_error("Rejected solids have been written to "
                                 "rejectedConversion.stp. Conversion terminated!");
    }
    std::cout << " > Converting " << solidObjList.size() << " solid(s)" << std::endl;
    auto voidCell = VoidCellManager{inputConfig}(solidObjList);
    std::cout << "   - Writing MC input file" << std::endl;
    Writer{inputConfig}(compoundList, voidCell);
}

McCAD::Conversion::Convert::Impl::~Impl(){
}

void
McCAD::Conversion::Convert::Impl::getGeomData(){
    // Loop over inputhapesMap and create compound objects.
    TaskQueue<Policy::Parallel> taskQueue;
    Standard_Integer counter = 0;
    for(const auto& member : inputShapesMap){
        taskQueue.submit([this, counter, &member](){
            std::shared_ptr<Geometry::Impl::Compound> compoundObj =
                    std::make_unique<Geometry::Impl::Compound>(
                    std::get<0>(member), std::get<1>(member));
            Decomposition::Preprocessor{inputConfig}(compoundObj);
            compoundObj->compoundID = counter;
            compoundList.push_back(std::move(compoundObj));
        });
        ++counter;
    }
    taskQueue.complete();
    // Loop over list of compounds, add solid ID, compoundID, and add rejected solids to list.
    counter = 0;
    for(Standard_Integer i = 0; i < compoundList.size(); ++i){
        for(const auto& solidObj : compoundList[i]->solidsList){
            solidObj->accessSImpl()->solidID = counter;
            solidObj->accessSImpl()->compoundID = compoundList[i]->compoundID;
            solidObjList.push_back(solidObj);
            ++counter;
        }
        if(compoundList[i]->rejectedInputShapesList->Length() > 0)
            rejectCondition = Standard_True;
            rejectConversion.push_back(std::make_tuple(
                                           compoundList[i]->compoundName,
                                           *compoundList[i]->rejectedInputShapesList));
    }
}
