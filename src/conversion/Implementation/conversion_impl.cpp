// C++
#include <iostream>
#include <map>
// McCAD
#include "conversion_impl.hpp"
#include "stepreader.hpp"
#include "stepwriter.hpp"
#include "inputdata_impl.hpp"
#include "preprocessor.hpp"
#include "TaskQueue.hpp"
#include "voidCellManager.hpp"
#include "conversionWriter.hpp"

McCAD::Conversion::Convert::Impl::Impl(IO::InputConfig& inputConfig) :
    inputConfig{inputConfig}{
    inputConfig.readConversion = Standard_True;
    Standard_Integer componentCounter{0}, solidCounter{0};
    for(Standard_Integer i = 0; i < inputConfig.conversionFileNames.size(); ++i){
        inputConfig.conversionFileName = inputConfig.conversionFileNames[i];
        IO::STEPReader reader{inputConfig}; //inputConfig.conversionFileName
        inputShapesMap = reader.getInputData().accessImpl()->inputShapesMap;
        if (!inputShapesMap.size() > 0)
            throw std::runtime_error("Error loading STEP file, " + inputConfig.conversionFileName);
        std::cout << "> Found " << inputShapesMap.size() <<
                     " shapes(s) in the input STEP file" << std::endl;
        solidCounter += getGeomData(inputConfig.materialsInfo[i], componentCounter, solidCounter);
        if (rejectCondition){
            // Write rejected solids to a STEP file.
            General::InputData outputData;
            outputData.accessImpl()->outputShapesMap = rejectConversion;
            inputConfig.outputFileName = inputConfig.rejectedConvFileNames[i];
            McCAD::IO::STEPWriter{inputConfig, outputData};
            throw std::runtime_error("Rejected solids have been written to "
                                     "rejectedConversion.stp. Conversion terminated!");
        }
        componentCounter += inputShapesMap.size();
    }
    std::cout << " > Converting " << compoundList.size() << " solid(s)" << std::endl;
    auto voidCell = VoidCellManager{inputConfig}(solidObjList);
    std::cout << "   - Writing MC input file" << std::endl;
    Writer{inputConfig}(compoundList, voidCell);
}

McCAD::Conversion::Convert::Impl::~Impl(){
}

Standard_Integer
McCAD::Conversion::Convert::Impl::getGeomData(const std::tuple<std::string, Standard_Real>& matInfo,
                                              const Standard_Integer& componentIndex,
                                              const Standard_Integer& solidIndex){
    // Loop over inputhapesMap and create compound objects.
    Standard_Integer index{componentIndex};
    std::map<Standard_Integer, std::shared_ptr<Geometry::Impl::Compound>> tempCompoundMap;
    TaskQueue<Policy::Parallel> taskQueue;
    for(const auto& member : inputShapesMap){
        taskQueue.submit([this, index, matInfo, &tempCompoundMap, &member](){
            std::shared_ptr<Geometry::Impl::Compound> compoundObj =
                    std::make_unique<Geometry::Impl::Compound>(
                    std::get<0>(member), std::get<1>(member));
            Decomposition::Preprocessor{inputConfig}(compoundObj);
            compoundObj->compoundID = index;
            compoundObj->matInfo = matInfo;
            tempCompoundMap[index] = std::move(compoundObj);
        });
        ++index;
    }
    taskQueue.complete();
    // Loop over list of compounds, add solid ID, compoundID, and add rejected solids to list.
    index = solidIndex;
    for(const auto& member : tempCompoundMap){
        for(const auto& solidObj : member.second->solidsList){
            solidObj->accessSImpl()->solidID = index;
            solidObj->accessSImpl()->compoundID = member.second->compoundID;
            solidObjList.push_back(solidObj);
            ++index;
        }
        if(member.second->rejectedInputShapesList->Length() > 0){
            rejectCondition = Standard_True;
            rejectConversion.push_back(std::make_tuple(
                                           member.second->compoundName,
                                           *member.second->rejectedInputShapesList));
        }
        // Add compound to compound list.
        compoundList.push_back(member.second);
    }
    return solidIndex;
}
