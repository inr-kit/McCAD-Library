// McCAD
#include "SolidRepairer.hpp"
#include "SolidRebuilder.hpp"
// OCCT
#include <BRepCheck_Analyzer.hxx>

bool
McCAD::Decomposition::SolidRepairer::operator()(
        TopoDS_Solid& solid) const{
    if(BRepCheck_Analyzer{solid, true}.IsValid())
        return true;

    auto rebuiltSolid = SolidRebuilder{}(solid);
    if(rebuiltSolid){
        solid = *rebuiltSolid;
        return true;
    }

    return false;
}
