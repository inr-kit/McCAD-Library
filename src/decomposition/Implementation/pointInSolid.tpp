//C++
//McCAD
#include "pointInSolid.hpp"
//OCC
#include <BRepBndLib.hxx>
#include <Bnd_OBB.hxx>

template <typename solidType>
Standard_Boolean
McCAD::Decomposition::PointInSolid::operator()(const solidType& solid,
                                               const gp_Pnt& point){
    Bnd_OBB obb;
    BRepBndLib::AddOBB(solid, obb);
    return !obb.IsOut(point);
}
