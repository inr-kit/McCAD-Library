// C++
#include <iostream>
// McCAD
#include "conversion_impl.hpp"
#include "stepreader.hpp"
#include "inputdata_impl.hpp"
#include "heirarchyFlatter.hpp"
#include "preprocessor.hpp"
#include "TaskQueue.hpp"
#include "BVHCreator.hpp"
//#include "voidGenerator.hpp"

McCAD::Conversion::Convert::Impl::Impl(const IO::InputConfig& inputConfig) :
    splitInputSolidsList{std::make_shared<TopTools_HSequenceOfShape>()} ,
    rejectedInputSolidsList{std::make_shared<TopTools_HSequenceOfShape>()}{
    /*IO::STEPReader reader{inputConfig.inputFileName};
    auto inputData = reader.getInputData();
    auto& inputSolidsList = inputData.accessImpl()->inputSolidsList;
    if (!inputSolidsList->Length() > 0)
        throw std::runtime_error("Input solids list is empty!");
    std::cout << "> Found " << inputSolidsList->Length() <<
                 " solid(s) in the input step file" << std::endl;
    auto product = Tools::HeirarchyFlatter{}.flattenSolidHierarchy(
                inputSolidsList);
    splitInputSolidsList = std::move(product.first);
    rejectedInputSolidsList = std::move(product.second);*/
    Conversion::BVHCreator{inputConfig, splitInputSolidsList};
    //getGeomData();
    //getMatData();
    std::cout << " > Converting " << splitInputSolidsList->Length() <<
                 " solid(s)" << std::endl;
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
    for(const auto& shape : *splitInputSolidsList){
        taskQueue.submit([this, &shape](){
            auto solid = Decomposition::Preprocessor{}.perform(shape);
            if (std::holds_alternative<std::monostate>(solid)){
                rejectedInputSolidsList->Append(shape);}
        });
    }
    taskQueue.complete();
}

void
McCAD::Conversion::Convert::Impl::getMatData(){}

void
McCAD::Conversion::Convert::Impl::perform(){
}
