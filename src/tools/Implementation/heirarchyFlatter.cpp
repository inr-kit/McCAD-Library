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
McCAD::Tools::HeirarchyFlatter::operator()(
        const std::shared_ptr<TopTools_HSequenceOfShape>& inputShapesList){
    for(const auto& shape : *inputShapesList){
        process(shape);
    }
    return std::make_pair(std::move(splitInputSolidsList),
                          std::move(rejectedInputSolidsList));
}

McCAD::Tools::HeirarchyFlatter::output_pair
McCAD::Tools::HeirarchyFlatter::operator()(const TopoDS_Shape& shape){
    process(shape);
    return std::make_pair(std::move(splitInputSolidsList),
                          std::move(rejectedInputSolidsList));
}

// This is to be removed once compound object is implemented in conversion
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
    return std::make_pair(splitInputSolidsMap, rejectedInputSolidsMap);
}

void
McCAD::Tools::HeirarchyFlatter::process(const TopoDS_Shape& shape){
    switch(shape.ShapeType()){
    case TopAbs_COMPOUND:
        [[fallthrough]];
    case TopAbs_COMPSOLID:
        for(const auto& solid : detail::ShapeView<TopAbs_SOLID>{shape}){
            splitInputSolidsList->Append(solid);
        };
        break;
    case TopAbs_SOLID:
        splitInputSolidsList->Append(shape);
        break;
    default:
        rejectedInputSolidsList->Append(shape);
    }
}
