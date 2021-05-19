//McCAD
#include "solidObjCreator.hpp"
#include "solid_impl.hpp"
#include "facesListGenerator.hpp"

template<typename solidObjType>
std::shared_ptr<solidObjType>
McCAD::Decomposition::SolidObjCreator::createObj(const TopoDS_Shape& shape,
                                                 const Standard_Real& scalingFactor,
                                                 const Standard_Real& angularTolerance,
                                                 const Standard_Real& precision){
    std::shared_ptr<solidObjType> solidObj = std::make_shared<solidObjType>();
    solidObj->accessSImpl()->initiate(shape);
    solidObj->accessSImpl()->repairSolid(precision);
    solidObj->accessSImpl()->createOBB();
    solidObj->accessSImpl()->calcMeshDeflection(scalingFactor);
    solidObj->accessSImpl()->updateEdgesConvexity(angularTolerance, precision);
    solidObj->accessSImpl()->facesList = FacesListGenerator{}(*solidObj);
    solidObj->accessSImpl()->calcVolume();
    return solidObj;
}
