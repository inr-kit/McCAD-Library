#ifndef POINTINSOLID_HPP
#define POINTINSOLID_HPP

//C++
//McCAD
#include <Standard.hxx>
//OCC
#include <gp_Pnt.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shape.hxx>

namespace McCAD::Decomposition{
class PointInSolid{
public:
    PointInSolid() = default;

    template <typename solidType>
    Standard_Boolean operator()(const solidType& solid, const gp_Pnt& point);
};
}

#include "pointInSolid.tpp"

#endif //POINTINSOLID_HPP
