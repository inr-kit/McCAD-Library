//McCAD
#include "ShapeUtilities.hpp"
// OCC
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>

double
McCAD::Tools::calcVolume(const TopoDS_Shape& aSolidShape){
    // Volume in mm3
    GProp_GProps geometryProperties;
    BRepGProp::VolumeProperties(aSolidShape, geometryProperties);
    return geometryProperties.Mass();
}