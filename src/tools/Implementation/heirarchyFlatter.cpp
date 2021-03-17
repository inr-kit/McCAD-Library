// C++
#include <iostream>
// McCAD
#include <Standard.hxx>
#include "heirarchyFlatter.hpp"
#include "ShapeView.hpp"

McCAD::Tools::HeirarchyFlatter::HeirarchyFlatter() :
    splitInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}{
}

McCAD::Tools::HeirarchyFlatter::~HeirarchyFlatter(){
}

std::pair<std::unique_ptr<TopTools_HSequenceOfShape>,
std::unique_ptr<TopTools_HSequenceOfShape>>
McCAD::Tools::HeirarchyFlatter::flattenSolidHierarchy(
        const std::shared_ptr<TopTools_HSequenceOfShape>& inputSolidsList){
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
    std::cout << "   " << compSolid << " compound solid(s)" << std::endl;
    std::cout << "   " << solid << " solid(s)" << std::endl;
    std::cout << "   " << invalidShape << " invalid shape(s)" << std::endl;
    return std::make_pair(std::move(splitInputSolidsList),
                          std::move(rejectedInputSolidsList));
}
