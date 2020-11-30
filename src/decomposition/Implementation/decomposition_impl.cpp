// McCAD
#include "decomposition_impl.hpp"
#include "heirarchyFlatter.hpp"
#include "TaskQueue.hpp"
#include "torusConvertor.hpp"

McCAD::Decomposition::Decompose::Impl::Impl(const McCAD::General::InputData& inputData)
  : splitInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    resultSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedsubSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}{
    // Get input solids list from the Input Data object.
    auto& inputSolidsList = inputData.accessImpl()->inputSolidsList;
    if (!inputSolidsList->Length() > 0)
        throw std::runtime_error("Input solids list is empty!");
    std::cout << "> Found " << inputSolidsList->Length() <<
                 " solid(s) in the input step file" << std::endl;
    auto product = Tools::HeirarchyFlatter{}.flattenSolidHierarchy(inputSolidsList);
    splitInputSolidsList = std::move(product.first);
    rejectedInputSolidsList = std::move(product.second);
    std::cout << " > Decomposing " << splitInputSolidsList->Length() <<
                 " solid(s)" << std::endl;
    perform();
}

McCAD::Decomposition::Decompose::Impl::~Impl(){
}

void
McCAD::Decomposition::Decompose::Impl::perform(const TopoDS_Shape& shape){
    auto solid = Preprocessor{}.perform(shape);
    if (std::holds_alternative<std::monostate>(solid)){
        //std::cout << "empty variant" << std::endl;
        rejectedInputSolidsList->Append(shape);
        goto decompositionPass;
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
    // Pass as there is no solid to decompose;
    decompositionPass: ;
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
    std::cout << " > Results:" << std::endl;
    std::cout << "   - There are " << rejectedInputSolidsList->Length() <<
                 " rejected input solid(s)." << std::endl;
    std::cout << "   - There are " << rejectedsubSolidsList->Length() <<
                 " rejected subsolid(s)." << std::endl;
    std::cout << "   - There are " << resultSolidsList->Length() <<
                 " result solid(s)." << std::endl;
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
