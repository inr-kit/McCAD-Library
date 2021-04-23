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

McCAD::Tools::HeirarchyFlatter::output_pair
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

McCAD::Tools::HeirarchyFlatter::output_MapPair
McCAD::Tools::HeirarchyFlatter::operator()(
        const McCAD::Tools::HeirarchyFlatter::shape_Name& inputSolidsMap){
    Standard_Integer compSolid{0}, solid{0}, invalidShape{0};
    for(Standard_Integer i=0; i < inputSolidsMap.size(); ++i){
        TopoDS_Shape shape = std::get<0>(inputSolidsMap[i]);
        auto name = std::get<1>(inputSolidsMap[i]);
        switch(shape.ShapeType()){
        case TopAbs_COMPOUND:
            [[fallthrough]];
        case TopAbs_COMPSOLID:
            ++compSolid;
            for(const auto& solid : detail::ShapeView<TopAbs_SOLID>{shape}){
                splitInputSolidsMap.push_back(std::make_tuple(solid, name, i));
            };
            break;
        case TopAbs_SOLID:
            ++solid;
            splitInputSolidsMap.push_back(std::make_tuple(shape, name, i));
            break;
        default:
            ++invalidShape;
            rejectedInputSolidsMap.push_back(std::make_tuple(shape, name, i));
        }
    }
    std::cout << "   " << compSolid << " compound solid(s)" << std::endl;
    std::cout << "   " << solid << " solid(s)" << std::endl;
    std::cout << "   " << invalidShape << " invalid shape(s)" << std::endl;
    return std::make_pair(splitInputSolidsMap, rejectedInputSolidsMap);
}
