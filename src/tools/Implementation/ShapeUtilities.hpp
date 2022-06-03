#ifndef SHAPEUTILITIES_HPP
#define SHAPEUTILITIES_HPP

// OCC
#include <TopoDS_Shape.hxx>

namespace McCAD::Tools{

    double calcVolume(const TopoDS_Shape& aSolidShape);

}

#endif
