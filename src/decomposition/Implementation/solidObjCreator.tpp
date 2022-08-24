//McCAD
#include "solidObjCreator.hpp"
#include "solid_impl.hpp"
#include "facesListGenerator.hpp"

McCAD::Decomposition::SolidObjCreator::SolidObjCreator(const IO::InputConfig& inputConfig) :
    scalingFactor{inputConfig.scalingFactor}, angularTolerance{inputConfig.angularTolerance},
    precision{inputConfig.precision}, edgeTolerance{inputConfig.edgeTolerance},
    faceTolerance{inputConfig.faceTolerance}, parameterTolerance{inputConfig.parameterTolerance},
    distanceTolerance{inputConfig.distanceTolerance}{}

McCAD::Decomposition::SolidObjCreator::~SolidObjCreator(){}

/** ********************************************************************
* @brief   A function that creates a solid object.
* @param   shape is an OCCT shape.
* @return  A McCAD solid object.
* @date    31/12/2020
* @author  Moataz Harb & Christian Wegmann
* **********************************************************************/
template<typename solidObjType>
std::shared_ptr<solidObjType>
McCAD::Decomposition::SolidObjCreator::createObj(const TopoDS_Shape& shape){
    std::shared_ptr<solidObjType> solidObj = std::make_shared<solidObjType>();
    solidObj->accessSImpl()->initiate(shape);
    solidObj->accessSImpl()->repairSolid(precision, faceTolerance);
    solidObj->accessSImpl()->createBB();
    solidObj->accessSImpl()->calcMeshDeflection(scalingFactor);
    solidObj->accessSImpl()->updateEdgesConvexity(angularTolerance, precision);
    solidObj->accessSImpl()->facesList = FacesListGenerator{}(solidObj,
                                                              precision,
                                                              edgeTolerance,
                                                              faceTolerance,
                                                              angularTolerance,
                                                              distanceTolerance,
                                                              parameterTolerance);
    solidObj->accessSImpl()->calcVolume();
    return solidObj;
}
