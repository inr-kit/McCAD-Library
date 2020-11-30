#include "SolidRepairer.hpp"

// C++
#include <algorithm>

// OCC
#include <TopoDS.hxx>

template<typename Container>
bool
McCAD::Decomposition::SolidRepairer::operator()(
        Container& solids) const{
    return std::all_of(
                solids.begin(),
                solids.end(),
                [this](TopoDS_Shape& solid){
                    return this->operator()(TopoDS::Solid(solid));
                });
}
