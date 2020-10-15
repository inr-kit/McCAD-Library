// C++
#include <iostream>
// McCAD
#include "conversion_impl.hpp"
#include "heirarchyFlatter.hpp"
#include "preprocessor.hpp"
#include "TaskQueue.hpp"

McCAD::Conversion::Convert::Impl::Impl(const IO::InputConfig& inputConfig,
                                       const General::InputData& inputData) :
    splitInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()} ,
    rejectedInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}{
    // Get input solids list from the Input Data object.
    auto& inputSolidsList = inputData.accessImpl()->inputSolidsList;
    if (inputSolidsList->Length() > 0){
        std::cout << "> Found " << inputSolidsList->Length() <<
                     " solid(s) in the input step file" << std::endl;
        std::cout << " > Spliting compound input solids" << std::endl;
        // Split compound input solids.
        auto product = Tools::HeirarchyFlatter{}.flattenSolidHierarchy(
                    inputSolidsList);
        splitInputSolidsList = std::move(product.first);
        rejectedInputSolidsList = std::move(product.second);
        std::cout << "   - There are " << splitInputSolidsList->Length() <<
                     " solid(s) in the flattened solids heirarchy" <<std::endl;
        std::cout << " > Converting solid(s)" << std::endl;
        // Perform the conversion.
        perform();
      } else{
        throw std::runtime_error("Input solids list is empty!");
      }
}

McCAD::Conversion::Convert::Impl::~Impl(){
}

void
McCAD::Conversion::Convert::Impl::perform(const TopoDS_Shape& shape){
    auto solid = Decomposition::Preprocessor{}.perform(shape);
    if (std::holds_alternative<std::monostate>(solid)){
        //std::cout << "empty variant" << std::endl;
        rejectedInputSolidsList->Append(shape);
        goto conversionPass;
    }
    conversionPass: ;
}

void
McCAD::Conversion::Convert::Impl::perform(){
    TaskQueue<Policy::Parallel> taskQueue;
    for(const auto& shape : *splitInputSolidsList){
        taskQueue.submit([this, &shape](){perform(shape);});
    }
    taskQueue.complete();
    std::cout << " > Results:" << std::endl;
    std::cout << "   - There are " << rejectedInputSolidsList->Length() <<
                 " rejected input solid(s)." << std::endl;
}
