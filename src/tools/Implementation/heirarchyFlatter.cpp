// C++
#include <iostream>
// McCAD
#include "heirarchyFlatter.hpp"
#include "ShapeView.hpp"
// OCC
#include <Standard.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Compound.hxx>

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

