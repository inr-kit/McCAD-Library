#include "SolidRepairer.hpp"

// McCAD
#include "SolidRebuilder.hpp"

// OCC
#include <BRepCheck_Analyzer.hxx>

bool
McCAD::Decomposition::SolidRepairer::operator()(
        TopoDS_Solid& solid) const{
    if(BRepCheck_Analyzer{solid, Standard_True}.IsValid())
        return true;

    auto rebuiltSolid = SolidRebuilder{}(solid);
    if(rebuiltSolid){
        solid = *rebuiltSolid;
        return true;
    }

    return false;
}
