//McCAD
#include "solidObjCreator.hpp"
#include "solid_impl.hpp"
#include "facesListGenerator.hpp"

McCAD::Decomposition::SolidObjCreator::SolidObjCreator(const IO::InputConfig& inputConfig) :
    scalingFactor{inputConfig.scalingFactor}, angularTolerance{inputConfig.angularTolerance},
    precision{inputConfig.precision}, edgeTolerance{inputConfig.edgeTolerance},
    faceTolerance{inputConfig.faceTolerance}, distanceTolerance{inputConfig.distanceTolerance}{}

McCAD::Decomposition::SolidObjCreator::~SolidObjCreator(){}

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
                                                              distanceTolerance);
    solidObj->accessSImpl()->calcVolume();
    return solidObj;
}
