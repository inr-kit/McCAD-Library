// McCAD
#include "decomposition_impl.hpp"

McCAD::Decomposition::Decompose::Impl::Impl(const McCAD::General::InputData& inputData)
  : splitInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    resultSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedsubSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}{
  // Get input solids list from the Input Data object.
  auto& inputSolidsList = inputData.accessImpl()->inputSolidsList;
  if (inputSolidsList->Length() > 0){
      std::cout << "> Found " << inputSolidsList->Length() <<
                   " solid(s) in the input step file." << std::endl;
      std::cout << " > Spliting compound input solids" << std::endl;
      // Split compound input solids.
      flattenSolidHierarchy(inputSolidsList);
      std::cout << "   - There are " << splitInputSolidsList->Length() <<
                   " solid(s) in the flattened solids heirarchy." <<std::endl;
      std::cout << " > Decomposing solid(s)" << std::endl;
      // Perform the decomposition.
      perform();
    } else{
      throw std::runtime_error("Input solids list is empty!.");
    }
}

McCAD::Decomposition::Decompose::Impl::~Impl(){
}

void
McCAD::Decomposition::Decompose::Impl::flattenSolidHierarchy(
        const Handle_TopTools_HSequenceOfShape& inputSolidsList){
    for(const auto& shape : *inputSolidsList){
        switch(shape.ShapeType()){
        case TopAbs_COMPOUND:
            [[fallthrough]];
        case TopAbs_COMPSOLID:
            std::cout << "   - Found a compound solid" << std::endl;
            for(const auto& solid : ShapeView<TopAbs_SOLID>{shape}){
                splitInputSolidsList->Append(solid);
            };
            break;
        case TopAbs_SOLID:
            std::cout << "   - Found a solid" << std::endl;
            splitInputSolidsList->Append(shape);
            break;
        default:
            std::cout << "   - Found invalid shape" << std::endl;
            rejectedInputSolidsList->Append(shape);
        }
    }
}

void
McCAD::Decomposition::Decompose::Impl::perform(){
    for(const auto& shape : *splitInputSolidsList){
        auto solid = Preprocessor{}.perform(shape);
        if (std::holds_alternative<std::monostate>(solid)){
            rejectedInputSolidsList->Append(shape);
        } else{
            // Using switch for now. Should be separated in a separate class an called
            // for each specific type of solid object.
            switch (Standard_Integer(solid.index())){
            default:
                auto& solidImpl = *std::get<1>(solid)->accessSImpl();
                // Perform decomposition on the repaired solid.
                std::cout << "   - Decomposing solid" << std::endl;
                if (DecomposeSolid{}.accessDSImpl()->perform(solidImpl)){
                    for(const auto& resultSolid : *solidImpl.splitSolidList){
                        resultSolidsList->Append(resultSolid);
                    }
                    if (solidImpl.rejectedsubSolidsList->Length() >= 1){
                        for(const auto& rejectedSubSolid : *solidImpl.rejectedsubSolidsList){
                            rejectedsubSolidsList->Append(rejectedSubSolid);
                        }
                    }
                } else{
                    rejectedInputSolidsList->Append(shape);
                }
            }
        }
    }
    std::cout << "   - There are " << rejectedInputSolidsList->Length() <<
                 " rejected input solid(s)." << std::endl;
    std::cout << "   - There are " << resultSolidsList->Length() <<
                 " result solid(s)." << std::endl;
    std::cout << "   - There are " << rejectedsubSolidsList->Length() <<
                 " rejected subsolid(s)." << std::endl;
    rejectedInputSolidsList->Append(*rejectedsubSolidsList);
}
