// McCAD
#include "decomposition_impl.hpp"
#include "TaskQueue.hpp"
#include "torusConvertor.hpp"

McCAD::Decomposition::Decompose::Impl::Impl(const McCAD::General::InputData& inputData)
  : splitInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    resultSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedsubSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}{
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
      // Perform the decomposition.
      perform();
    } else{
      throw std::runtime_error("Input solids list is empty!");
    }
}

McCAD::Decomposition::Decompose::Impl::~Impl(){
}

void
McCAD::Decomposition::Decompose::Impl::flattenSolidHierarchy(
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
McCAD::Decomposition::Decompose::Impl::perform(const TopoDS_Shape& shape){
    auto solid = Preprocessor{}.perform(shape);
    if (std::holds_alternative<std::monostate>(solid)){
        //std::cout << "empty variant" << std::endl;
        rejectedInputSolidsList->Append(shape);
        return;
    }
    // Using switch for now. Should be separated in a separate class an called
    // for each specific type of solid object.
    switch (Standard_Integer(solid.index())){
    case solidType.planar:{
        std::cout << "   - Decomposing planar solid" << std::endl;
        if (DecomposeSolid{}.accessDSImpl()->operator()(
                    std::get<solidType.planar>(solid))){
            extractSolids(*std::get<solidType.planar>(solid)->accessSImpl());
        } else rejectedInputSolidsList->Append(shape);
        break;
    } case solidType.cylindrical:{
        std::cout << "   - Decomposing cylindrical solid" << std::endl;
        if (DecomposeSolid{}.accessDSImpl()->operator()(
                    std::get<solidType.cylindrical>(solid))){
            extractSolids(*std::get<solidType.cylindrical>(solid)->accessSImpl());
        } else rejectedInputSolidsList->Append(shape);
        break;
    } case solidType.toroidal:{
        std::cout << "   - Decomposing toroidal solid" << std::endl;
        if (DecomposeSolid{}.accessDSImpl()->operator()(
                    std::get<solidType.toroidal>(solid))){
            // Convert tori to cylinders.
            TorusConvertor{}(*std::get<solidType.toroidal>(solid)->accessSImpl());
            extractSolids(*std::get<solidType.toroidal>(solid)->accessSImpl());
        } else rejectedInputSolidsList->Append(shape);
        break;
    } default:
        std::cout << "   - Processing of solids with non-planar surfaces"
                     " is not yet supported!.\n     Solid will be added"
                     " to rejected solids file" << std::endl;
        rejectedInputSolidsList->Append(shape);
    }
}

void
McCAD::Decomposition::Decompose::Impl::perform(){
    TaskQueue<Policy::Parallel> taskQueue;
    for(const auto& shape : *splitInputSolidsList){
        taskQueue.submit([this, &shape](){
            perform(shape);
        });
    }
    taskQueue.complete();
    std::cout << " > Result" << std::endl;
    std::cout << "   - There are " << rejectedInputSolidsList->Length() <<
                 " rejected input solid(s)." << std::endl;
    std::cout << "   - There are " << resultSolidsList->Length() <<
                 " result solid(s)." << std::endl;
    std::cout << "   - There are " << rejectedsubSolidsList->Length() <<
                 " rejected subsolid(s)." << std::endl;
    rejectedInputSolidsList->Append(*rejectedsubSolidsList);
}

void
McCAD::Decomposition::Decompose::Impl::extractSolids(
        const Geometry::Solid::Impl& solidImpl){
    for(const auto& resultSolid : *solidImpl.splitSolidList){
        resultSolidsList->Append(resultSolid);
    }
    if (solidImpl.rejectedsubSolidsList->Length() >= 1){
        for(const auto& rejectedSubSolid : *solidImpl.rejectedsubSolidsList){
            rejectedsubSolidsList->Append(rejectedSubSolid);
        }
    }
}
