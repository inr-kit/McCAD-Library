//McCAD
#include "ShapeUtilities.hpp"
// OCC
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>

/** ********************************************************************
* @brief    A function that calculates a given shape volume.
* @param    aSolidShape is a OCCT shape.
* @return   A calculated shape volume.
* @date     31/12/2020
* @modified 
* @author   Moataz Harb & Christian Wegmann
* **********************************************************************/
double
McCAD::Tools::calcVolume(const TopoDS_Shape& aSolidShape){
    // Volume in mm3
    GProp_GProps geometryProperties;
    BRepGProp::VolumeProperties(aSolidShape, geometryProperties);
    return geometryProperties.Mass();
}