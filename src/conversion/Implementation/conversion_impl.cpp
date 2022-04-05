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

/** ********************************************************************
* @brief   Executes McCAD conversion.
* @param   inputConfig is an object containing the values of the configuration parameters.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
McCAD::Conversion::Convert::Impl::Impl(IO::InputConfig& inputConfig) :
    inputConfig{ inputConfig }, debugLevel{inputConfig.debugLevel }{
    inputConfig.readConversion = true;
    int componentCounter{0}, solidCounter{0};
    for(int i = 0; i < inputConfig.conversionFileNames.size(); ++i){
        inputConfig.conversionFileName = inputConfig.conversionFileNames[i];
        IO::STEPReader reader{inputConfig}; //loads the solids from inputConfig.conversionFileName file.
        inputShapesMap = reader.getInputData().accessImpl()->inputShapesMap;
        if (!inputShapesMap.size() > 0)
            throw std::runtime_error("Error loading STEP file, " + inputConfig.conversionFileName);
        std::cout << "> Found " << inputShapesMap.size() <<
                     " shape(s) in the input STEP file" << std::endl;
        solidCounter = getGeomData(inputConfig.materialsInfo[i], componentCounter, solidCounter);
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

/** ********************************************************************
* @brief   Populates rthe components list with input solids.
* @param   matInfo is a tiple of material name and density
* @param   componentIndex is the order of components/compounds loaded so far from all input files.
* @param   solidIndex is the order of solids loaded so far from all input files.
* @return  appended solid index to be used as ID for new solids.
* @date    01/01/2021
* @author  Moataz Harb
* **********************************************************************/
int
McCAD::Conversion::Convert::Impl::getGeomData(const std::tuple<std::string, double>& matInfo,
                                              const int& componentIndex,
                                              const int& solidIndex){
    // Loop over inputShapesMap and create compound objects.
    int index{componentIndex};
    std::map<int, std::shared_ptr<Geometry::Impl::Compound>> tempCompoundMap;
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
    // Loop over list of compounds; add solid ID, compoundID, and rejected solids to list.
    index = solidIndex;
    for(const auto& member : tempCompoundMap){
        for(const auto& solidObj : member.second->solidsList){
            solidObj->accessSImpl()->solidID = index;
            solidObj->accessSImpl()->compoundID = member.second->compoundID;
            solidObjList.push_back(solidObj);
            ++index;
        }
        if(member.second->rejectedInputShapesList->Length() > 0){
            rejectCondition = true;
            if (debugLevel >= 1) {
                std::cout << member.second->compoundName << std::endl;
            }
            rejectConversion.push_back(std::make_tuple(
                                           member.second->compoundName,
                                           *member.second->rejectedInputShapesList));
        }
        // Add compound to compound list.
        compoundList.push_back(member.second);
    }
    return index + 1;
}
