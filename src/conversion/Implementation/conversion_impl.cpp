// C++
#include <iostream>
// McCAD
#include "conversion_impl.hpp"
#include "stepreader.hpp"
#include "inputdata_impl.hpp"
#include "heirarchyFlatter.hpp"
#include "preprocessor.hpp"
#include "TaskQueue.hpp"
#include "voidCellManager.hpp"

McCAD::Conversion::Convert::Impl::Impl(const IO::InputConfig& inputConfig){
    IO::STEPReader reader{inputConfig.conversionFileName};
    auto inputData = reader.getInputData();
    auto inputSolidsMap = inputData.accessImpl()->inputSolidsMap;
    if (inputSolidsMap.size() == 0)
        throw std::runtime_error("Input solids list is empty!");
    std::cout << "> Found " << inputSolidsMap.size() <<
                 " shapes(s) in the input STEP file" << std::endl;
    auto product = Tools::HeirarchyFlatter{}(inputSolidsMap);
    acceptedInputSolidsList = product.first;
    rejectedInputSolidsList = product.second;
    std::cout << "returned, " << acceptedInputSolidsList.size() << ", "
              << rejectedInputSolidsList.size() << std::endl;
    if (rejectedInputSolidsList.size() != 0){
        rejectCondition = Standard_True;
    }
    getGeomData();
    if (rejectCondition){
        std::cout << "> Input STEP file contains " << rejectedInputSolidsList.size() <<
                     "shapes that are not yet supported!" << std::endl;
        throw std::runtime_error("Conversion terminated!");
    }
    std::cout << " > Converting " << acceptedInputSolidsList.size() << " solid(s)"
              << std::endl;
    if (inputConfig.voidGeneration){
        std::cout << "   - Generating void" << std::endl;
        auto voidCell = VoidCellManager{inputConfig.BVHVoid,
                inputConfig.maxSolidsPerVoidCell}(solidObjList);
    }
    // call writer to write solids and void cells.
}

McCAD::Conversion::Convert::Impl::~Impl(){
}

void
McCAD::Conversion::Convert::Impl::getGeomData(){
    Standard_Integer counter = 0;
    TaskQueue<Policy::Parallel> taskQueue;
    for(const auto& shape : acceptedInputSolidsList){
        ++counter;
        taskQueue.submit([this, &shape, counter](){
            auto solid = Decomposition::Preprocessor{}.perform(std::get<0>(shape));
            if (std::holds_alternative<std::monostate>(solid)){
                rejectedInputSolidsList.push_back(shape);
                rejectCondition = Standard_True;
            } else{
                switch (Standard_Integer(solid.index())){
                case solidType.planar:{
                    auto solidObj = std::get<solidType.planar>(solid);
                    solidObj->accessSImpl()->solidID = counter;
                    solidObj->accessSImpl()->originalID = std::get<2>(shape);
                    solidObj->accessSImpl()->solidName = std::get<1>(shape);
                    solidObjList.push_back(solidObj);
                    break;
                } case solidType.cylindrical:{
                    auto solidObj = std::get<solidType.cylindrical>(solid);
                    solidObj->accessSImpl()->solidID = counter;
                    solidObj->accessSImpl()->originalID = std::get<2>(shape);
                    solidObj->accessSImpl()->solidName = std::get<1>(shape);
                    solidObjList.push_back(solidObj);
                    break;
                } case solidType.toroidal:{
                    auto solidObj = std::get<solidType.toroidal>(solid);
                    solidObj->accessSImpl()->solidID = counter;
                    solidObj->accessSImpl()->originalID = std::get<2>(shape);
                    solidObj->accessSImpl()->solidName = std::get<1>(shape);
                    solidObjList.push_back(solidObj);
                    break;
                } default:
                    rejectedInputSolidsList.push_back(shape);
                    rejectCondition = Standard_True;
                }
            };
        });
    }
    taskQueue.complete();
}
