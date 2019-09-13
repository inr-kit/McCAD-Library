// McCAD
#include "decomposition_impl.hpp"
// C++
#include <deque>

McCAD::Decomposition::Decompose::Impl::Impl(const McCAD::General::InputData& inputData)
  : splitInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    resultSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedsubSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}{
  // Get input solids list from the Input Data object.
  auto& inputSolidsList = inputData.accessImpl()->inputSolidsList;
  if (inputSolidsList->Length() > 0)
    {
      std::cout << "   - Found " << inputSolidsList->Length() << " solid(s) in the input file." << std::endl;
      std::cout << " > Spliting compound input solids" << std::endl;
      // Split compound input solids.
      flattenSolidHierarchy(inputSolidsList);
      std::cout << "   - There are " << splitInputSolidsList->Length() << " solid(s) in the split solids list." <<std::endl;
      std::cout << " > Decomposing solid(s)" << std::endl;
      // Perform the decomposition.
      perform();
    }
  else
    {
      // Throw an exception.
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
            for(const auto& solid : ShapeView<TopAbs_SOLID>{shape})
                splitInputSolidsList->Append(solid);
            break;
        case TopAbs_SOLID:
            splitInputSolidsList->Append(shape);
            break;
        default:
            throw std::runtime_error{
                "Shape can only be COMPOUND, COMPSOLID or SOLID"
            };
        }
    }
}

void
McCAD::Decomposition::Decompose::Impl::perform(){
    // Loop over the solids in the split solids list and perform the decomposition
    for(const auto& shape : *splitInputSolidsList){
        Solid::Impl solidImpl;
        solidImpl.initiate(shape);
        // If the solid has spline or tori surfaces it cannot be processed by the current version of the code.
        if(solidImpl.isTorus
                || solidImpl.isSpline){
            rejectedInputSolidsList->Append(
                        solidImpl.solid);
        }else{
            // Repair the geometry of solid
            solidImpl.repairSolid();
            // Perform decomposition on the repaired solid.
            std::cout << "   - Decomposing next solid" << std::endl;
            if(DecomposeSolid{}.accessDSImpl()->perform(solidImpl)){
                resultSolidsList->Append(
                            *solidImpl.splitSolidList);
                rejectedsubSolidsList->Append(
                            *solidImpl.rejectedsubSolidsList);
            }else{
                rejectedInputSolidsList->Append(
                            solidImpl.solid);
            }
        }
    }
    std::cout << "   - There are " << rejectedInputSolidsList->Length() << " rejected solid(s)."<< std::endl;
    std::cout << "   - There are " << resultSolidsList->Length() << " result solid(s)." << std::endl;
    std::cout << "   - There are " << rejectedsubSolidsList->Length() << " rejected subsolid(s)." << std::endl;
}
