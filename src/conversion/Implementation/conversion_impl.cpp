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

McCAD::Conversion::Convert::Impl::Impl(const IO::InputConfig& inputConfig)
    : acceptedInputSolidsList{std::make_shared<TopTools_HSequenceOfShape>()},
      rejectedInputSolidsList{std::make_shared<TopTools_HSequenceOfShape>()}{
    IO::STEPReader reader{inputConfig.conversionFileName};
    auto inputData = reader.getInputData();
    auto& inputSolidsList = inputData.accessImpl()->inputSolidsList;
    if (inputSolidsList->IsEmpty())
        throw std::runtime_error("Input solids list is empty!");
    std::cout << "> Found " << inputSolidsList->Length() <<
                 " solid(s) in the input step file" << std::endl;
    auto product = Tools::HeirarchyFlatter{}.flattenSolidHierarchy(
                inputSolidsList);
    acceptedInputSolidsList = std::move(product.first);
    rejectedInputSolidsList = std::move(product.second);
    std::cout << " > Converting " << acceptedInputSolidsList->Length() <<
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
    for(const auto& shape : *acceptedInputSolidsList){
        taskQueue.submit([this, &shape](){
            auto solid = Decomposition::Preprocessor{}.perform(shape);
            if (std::holds_alternative<std::monostate>(solid)){
                rejectedInputSolidsList->Append(shape);
            } else{
                switch (Standard_Integer(solid.index())){
                case solidType.planar:{
                    solidsList.push_back(std::get<solidType.planar>(solid));
                    break;
                } case solidType.cylindrical:{
                    solidsList.push_back(std::get<solidType.cylindrical>(solid));
                    break;
                } case solidType.toroidal:{
                    solidsList.push_back(std::get<solidType.toroidal>(solid));
                    break;
                } default:
                    rejectedInputSolidsList->Append(shape);
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
