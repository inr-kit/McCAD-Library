// C++
#include <iostream>
// McCAD
#include "heirarchyFlatter.hpp"
#include "ShapeView.hpp"
// OCCT
#include <TopoDS_Solid.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Compound.hxx>

McCAD::Tools::HeirarchyFlatter::HeirarchyFlatter() :
    splitInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()},
    rejectedInputSolidsList{std::make_unique<TopTools_HSequenceOfShape>()}{
}

McCAD::Tools::HeirarchyFlatter::~HeirarchyFlatter(){
}

/** ********************************************************************
* @brief    An operator that flattens compound shapes.
* @param    inputShapesList is a list of OCCT shapes.
* @return   A pair of lists containing the accepted and rejected solids.
* @date     31/12/2020
* @modified 
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
McCAD::Tools::HeirarchyFlatter::output_pair
McCAD::Tools::HeirarchyFlatter::operator()(
        const std::shared_ptr<TopTools_HSequenceOfShape>& inputShapesList){
    for(const auto& shape : *inputShapesList){
        process(shape);
    }
    return std::make_pair(std::move(splitInputSolidsList),
                          std::move(rejectedInputSolidsList));
}

/** ********************************************************************
* @brief    An operator that flattens compound shapes.
* @param    shape is an OCCT shape.
* @return   A pair of lists containing the accepted and rejected solids.
* @date     31/12/2020
* @modified 
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
McCAD::Tools::HeirarchyFlatter::output_pair
McCAD::Tools::HeirarchyFlatter::operator()(const TopoDS_Shape& shape){
    process(shape);
    return std::make_pair(std::move(splitInputSolidsList),
                          std::move(rejectedInputSolidsList));
}

/** ********************************************************************
* @brief    A function that processes shapes.
* @param    shape is a OCCT shape.
* @date     31/12/2020
* @modified 
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
void
McCAD::Tools::HeirarchyFlatter::process(const TopoDS_Shape& shape){
    switch(shape.ShapeType()){
    case TopAbs_COMPOUND:
        [[fallthrough]];
    case TopAbs_COMPSOLID:
        for(const auto& solid : detail::ShapeView<TopAbs_SOLID>{shape}){
            splitInputSolidsList->Append(solid);
        };
        if(splitInputSolidsList->Length() == 0){
            splitInputSolidsList->Clear();
            rejectedInputSolidsList->Append(shape);
        }
        break;
    case TopAbs_SOLID:
        splitInputSolidsList->Append(shape);
        break;
    default:
        rejectedInputSolidsList->Append(shape);
    }
}

