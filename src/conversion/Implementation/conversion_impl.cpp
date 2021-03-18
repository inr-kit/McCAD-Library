// C++
#include <iostream>
// McCAD
#include "conversion_impl.hpp"
#include "stepreader.hpp"
#include "inputdata_impl.hpp"
#include "heirarchyFlatter.hpp"
#include "preprocessor.hpp"
#include "TaskQueue.hpp"
//#include "voidCellManager.hpp"

McCAD::Conversion::Convert::Impl::Impl(const IO::InputConfig& inputConfig){
    IO::STEPReader reader{inputConfig.conversionFileName};
    auto inputData = reader.getInputData();
    auto& inputSolidsList = inputData.accessImpl()->zinputSolidsList;
    if (inputSolidsList.size() == 0)
        throw std::runtime_error("Input solids list is empty!");
    std::cout << "> Found " << inputSolidsList.size() <<
                 " shapes(s) in the input STEP file" << std::endl;
    auto product = Tools::HeirarchyFlatter{}(inputSolidsList);
    acceptedInputSolidsList = std::move(product.first);
    rejectedInputSolidsList = std::move(product.second);
    std::cout << " > Converting " << acceptedInputSolidsList.size() <<
                 " solid(s)" << std::endl;
    getGeomData();
    getMatData();
    if (inputConfig.voidGeneration){
        std::cout << "   - Generating void" << std::endl;
        //Conversion::Impl::VoidGenerator{splitInputSolidsList};
    }
    //perform();
}

McCAD::Conversion::Convert::Impl::~Impl(){
}

void
McCAD::Conversion::Convert::Impl::getGeomData(){
    TaskQueue<Policy::Parallel> taskQueue;
    for(const auto& shape : acceptedInputSolidsList){
        taskQueue.submit([this, &shape](){
            auto solid = Decomposition::Preprocessor{}.perform(std::get<0>(shape));
            if (std::holds_alternative<std::monostate>(solid)){
                rejectedInputSolidsList.push_back(shape);
            } else{
                switch (Standard_Integer(solid.index())){
                case solidType.planar:{
                    auto solidObj = std::get<solidType.planar>(solid);
                    solidObj->accessSImpl()->solidName = std::get<1>(shape);
                    solidsList.push_back(solidObj);
                    break;
                } case solidType.cylindrical:{
                    auto solidObj = std::get<solidType.cylindrical>(solid);
                    solidObj->accessSImpl()->solidName = std::get<1>(shape);
                    solidsList.push_back(solidObj);
                    break;
                } case solidType.toroidal:{
                    auto solidObj = std::get<solidType.toroidal>(solid);
                    solidObj->accessSImpl()->solidName = std::get<1>(shape);
                    solidsList.push_back(solidObj);
                    break;
                } default:
                    rejectedInputSolidsList.push_back(shape);
                }
            };
        });
    }
    taskQueue.complete();
}

void
McCAD::Conversion::Convert::Impl::getMatData(){}

void
McCAD::Conversion::Convert::Impl::perform(){
}
