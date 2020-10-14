// C++
#include <iostream>
// McCAD
#include "conversion_impl.hpp"
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
        flattenSolidHierarchy(inputSolidsList);
        std::cout << "   - There are " << splitInputSolidsList->Length() <<
                     " solid(s) in the flattened solids heirarchy" <<std::endl;
        std::cout << " > Decomposing solid(s)" << std::endl;
        // Perform the conversion.
        perform();
      } else{
        throw std::runtime_error("Input solids list is empty!");
      }
}

McCAD::Conversion::Convert::Impl::~Impl(){
}

void
McCAD::Conversion::Convert::Impl::flattenSolidHierarchy(
        const Handle_TopTools_HSequenceOfShape& inputSolidsList){
    Standard_Integer compSolid{0}, solid{0}, invalidShape{0};
    for(const auto& shape : *inputSolidsList){
        switch(shape.ShapeType()){
        case TopAbs_COMPOUND:
            [[fallthrough]];
        case TopAbs_COMPSOLID:
            ++compSolid;
            for(const auto& solid : detail::ShapeView<TopAbs_SOLID>{shape}){
                splitInputSolidsList->Append(solid);
            };
            break;
        case TopAbs_SOLID:
            ++solid;
            splitInputSolidsList->Append(shape);
            break;
        default:
            ++invalidShape;
            rejectedInputSolidsList->Append(shape);
        }
    }
    std::cout << "   - Found " << compSolid << " compound solid(s)" << std::endl;
    std::cout << "   - Found " << solid << " solid(s)" << std::endl;
    std::cout << "   - Found " << invalidShape << " invalid shape(s)" << std::endl;
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
